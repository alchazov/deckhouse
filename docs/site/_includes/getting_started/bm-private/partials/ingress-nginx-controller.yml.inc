# [<en>] Section containing the parameters of Nginx Ingress controller.
# [<en>] https://deckhouse.io/documentation/v1/modules/402-ingress-nginx/cr.html
# [<ru>] Секция, описывающая параметры Nginx Ingress controller.
# [<ru>] https://deckhouse.ru/documentation/v1/modules/402-ingress-nginx/cr.html
apiVersion: deckhouse.io/v1
kind: IngressNginxController
metadata:
  name: nginx
spec:
  # [<en>] The name of the Ingress class to use with the Ingress nginx controller.
  # [<ru>] Имя Ingress-класса для обслуживания Ingress NGINX controller.
  ingressClass: nginx
  # [<en>] The way traffic goes to cluster from the outer network.
  # [<ru>] Способ поступления трафика из внешнего мира.
  inlet: HostPort
  hostPort:
    httpPort: 80
    httpsPort: 443
  # [<en>] Describes on which nodes the component will be located.
  # [<ru>] Описывает, на каких узлах будет находиться компонент.
  # [<en>] You might consider changing this.
  # [<ru>] Возможно, захотите изменить.
  nodeSelector:
    node-role.kubernetes.io/master: ""
  tolerations:
  - operator: Exists
