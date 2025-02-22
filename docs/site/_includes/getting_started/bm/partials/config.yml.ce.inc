# [<en>] General cluster parameters.
# [<en>] https://deckhouse.io/documentation/v1/installing/configuration.html#clusterconfiguration
# [<ru>] Секция с общими параметрами кластера.
# [<ru>] https://deckhouse.ru/documentation/v1/installing/configuration.html#clusterconfiguration
apiVersion: deckhouse.io/v1
kind: ClusterConfiguration
clusterType: Static
# [<en>] Address space of the cluster's Pods.
# [<ru>] Адресное пространство подов кластера.
podSubnetCIDR: 10.111.0.0/16
# [<en>] Address space of the cluster's services.
# [<ru>] Адресное пространство сети сервисов кластера.
serviceSubnetCIDR: 10.222.0.0/16
kubernetesVersion: "Automatic"
# [<en>] Cluster domain (used for local routing).
# [<ru>] Домен кластера.
clusterDomain: "cluster.local"
---
# [<en>] Section for bootstrapping the Deckhouse cluster.
# [<en>] https://deckhouse.io/documentation/v1/installing/configuration.html#initconfiguration
# [<ru>] Секция первичной инициализации кластера Deckhouse.
# [<ru>] https://deckhouse.ru/documentation/v1/installing/configuration.html#initconfiguration
apiVersion: deckhouse.io/v1
kind: InitConfiguration
deckhouse:
  releaseChannel: Stable
  configOverrides:
    global:
      modules:
        # [<en>] Template that will be used for system apps domains within the cluster.
        # [<en>] E.g., Grafana for %s.example.com will be available as 'grafana.example.com'.
        # [<en>] You can change it to your own or follow the steps in the guide and change it after installation.
        # [<ru>] Шаблон, который будет использоваться для составления адресов системных приложений в кластере.
        # [<ru>] Например, Grafana для %s.example.com будет доступна на домене 'grafana.example.com'.
        # [<ru>] Можете изменить на свой сразу, либо следовать шагам руководства и сменить его после установки.
        publicDomainTemplate: "%s.example.com"
    userAuthn:
      controlPlaneConfigurator:
        dexCAMode: DoNotNeed
      # [<ru>] Включение доступа к API-серверу Kubernetes через Ingress.
      # [<ru>] https://deckhouse.ru/documentation/v1/modules/150-user-authn/configuration.html#parameters-publishapi
      # [<en>] Enabling access to the API server through Ingress.
      # [<en>] https://deckhouse.io/documentation/v1/modules/150-user-authn/configuration.html#parameters-publishapi
      publishAPI:
        enable: true
        https:
          mode: Global
    # [<en>] Enable cni-cilium module
    # [<ru>] Включить модуль cni-cilium
    cniCiliumEnabled: true
    # [<en>] cni-cilium module settings
    # [<en>] https://deckhouse.io/documentation/v1/modules/021-cni-cilium/configuration.html
    # [<ru>] Настройки модуля cni-cilium
    # [<ru>] https://deckhouse.ru/documentation/v1/modules/021-cni-cilium/configuration.html
    cniCilium:
      tunnelMode: VXLAN
---
# [<en>] Section with the parameters of the static cluster.
# [<en>] https://deckhouse.io/documentation/v1/installing/configuration.html#staticclusterconfiguration
# [<ru>] Секция с параметрами статического кластера.
# [<ru>] https://deckhouse.ru/documentation/v1/installing/configuration.html#staticclusterconfiguration
apiVersion: deckhouse.io/v1
kind: StaticClusterConfiguration
# [<en>] List of internal cluster networks (e.g., '10.0.4.0/24'), which is
# [<en>] used for linking Kubernetes components (kube-apiserver, kubelet etc.).
# [<en>] If every node in cluster has only one network interface
# [<en>] StaticClusterConfiguration resource can be skipped.
# [<ru>] Список внутренних сетей узлов кластера (например, '10.0.4.0/24'), который
# [<ru>] используется для связи компонентов Kubernetes (kube-apiserver, kubelet...) между собой.
# [<ru>] Если каждый узел в кластере имеет только один сетевой интерфейс,
# [<ru>] ресурс StaticClusterConfiguration можно не создавать.
internalNetworkCIDRs:
- *!CHANGE_internalNetworkCIDRs*
