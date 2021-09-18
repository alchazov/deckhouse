package template

import (
	"fmt"
	"sync"
	"time"

	corev1 "k8s.io/api/core/v1"
	"k8s.io/client-go/informers"
	"k8s.io/client-go/tools/cache"
	"sigs.k8s.io/yaml"
)

func NewContext(factory informers.SharedInformerFactory, secretName, secretKey string, updateHandler UpdateHandler) *BashibleContext {
	c := BashibleContext{
		secretName:    secretName,
		secretKey:     secretKey,
		updateHandler: updateHandler,
	}

	c.subscribe(factory)

	return &c
}

type Context interface {
	Get(contextKey string) (map[string]interface{}, error)
}

type UpdateHandler interface {
	OnUpdate()
}

// BashibleContext manages bashible template context
type BashibleContext struct {
	rw sync.RWMutex

	// secretKey in secret to parse
	secretName string
	secretKey  string
	hasSynced  bool

	updateHandler UpdateHandler

	// data (taken by secretKey from secret) maps `contextKey` to `contextValue`,
	// the being arbitrary data for a combination of os, nodegroup, & kubeversion
	data map[string]interface{}
}

func (c *BashibleContext) subscribe(factory informers.SharedInformerFactory) chan struct{} {
	ch := make(chan map[string][]byte)
	stopInformer := make(chan struct{})

	// Launch the informer
	informer := factory.Core().V1().Secrets().Informer()
	go informer.Run(stopInformer)

	// Subscribe to updates
	informer.AddEventHandler(cache.FilteringResourceEventHandler{
		FilterFunc: secretMapFilter(c.secretName),
		Handler:    &secretEventHandler{ch},
	})

	// Store updates
	stopUpdater := make(chan struct{})
	go func() {
		for {
			select {
			case secretData := <-ch:
				c.update(secretData)
			case <-stopUpdater:
				close(stopInformer)
				return
			}
		}
	}()

	// Wait for the first sync of the informer cache, should not take long
	for !informer.HasSynced() {
		time.Sleep(200 * time.Millisecond)
	}

	return stopUpdater
}

func (c *BashibleContext) update(secretData map[string][]byte) {
	c.rw.Lock()
	defer c.rw.Unlock()

	value, ok := secretData[c.secretKey]
	if !ok {
		// server error, so we panic
		panic(fmt.Sprintf("absent key \"%s\" in secret %s\n", c.secretKey, c.secretName))
	}

	yaml.Unmarshal(value, &c.data)

	c.updateHandler.OnUpdate()
}

// Get retrieves a copy of context for the given secretKey.
//
// TODO In future, node group name will be passed instead of a secretKey.
func (c *BashibleContext) Get(contextKey string) (map[string]interface{}, error) {
	c.rw.RLock()
	defer c.rw.RUnlock()

	raw, ok := c.data[contextKey]
	if !ok {
		return nil, fmt.Errorf("context not found for secretKey \"%s\"", contextKey)
	}

	converted, ok := raw.(map[string]interface{})
	if !ok {
		return nil, fmt.Errorf("cannot convert context for secretKey \"%s\" to map[string]interface{}", contextKey)
	}

	copied := make(map[string]interface{})
	for k, v := range converted {
		copied[k] = v
	}

	return copied, nil
}

// secretMapFilter returns filtering function for single secret
func secretMapFilter(name string) func(obj interface{}) bool {
	return func(obj interface{}) bool {
		cm, ok := obj.(*corev1.Secret)
		if !ok {
			return false
		}
		return cm.ObjectMeta.Name == name
	}
}

type secretEventHandler struct {
	out chan map[string][]byte
}

func (x *secretEventHandler) OnAdd(obj interface{}) {
	secret := obj.(*corev1.Secret)
	x.out <- secret.Data
}

func (x *secretEventHandler) OnUpdate(oldObj, newObj interface{}) {
	secret := newObj.(*corev1.Secret)
	x.out <- secret.Data
}

func (x *secretEventHandler) OnDelete(obj interface{}) {
	// noop
}
