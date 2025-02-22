# [<en>] General cluster parameters.
# [<en>] https://deckhouse.io/documentation/v1/installing/configuration.html#clusterconfiguration
# [<ru>] Секция с общими параметрами кластера.
# [<ru>] https://deckhouse.ru/documentation/v1/installing/configuration.html#clusterconfiguration
apiVersion: deckhouse.io/v1
kind: ClusterConfiguration
clusterType: Cloud
cloud:
  provider: GCP
  # [<en>] A prefix of objects that are created in the cloud during the installation.
  # [<en>] You might consider changing this.
  # [<ru>] Префикс объектов, создаваемых в облаке при установке.
  # [<ru>] Возможно, захотите изменить.
  prefix: cloud-demo
# [<en>] Address space of the cluster's Pods.
# [<ru>] Адресное пространство Pod’ов кластера.
podSubnetCIDR: 10.111.0.0/16
# [<en>] Address space of the cluster's services.
# [<ru>] Адресное пространство для service’ов кластера.
serviceSubnetCIDR: 10.222.0.0/16
kubernetesVersion: "Automatic"
clusterDomain: "cluster.local"
---
# [<en>] Section for bootstrapping the Deckhouse cluster.
# [<en>] https://deckhouse.io/documentation/v1/installing/configuration.html#initconfiguration
# [<ru>] Секция первичной инициализации кластера Deckhouse.
# [<ru>] https://deckhouse.ru/documentation/v1/installing/configuration.html#initconfiguration
apiVersion: deckhouse.io/v1
kind: InitConfiguration
deckhouse:
  imagesRepo: registry.deckhouse.io/deckhouse/ee
  # [<en>] A special string with your token to access Docker registry (generated automatically for your license token).
  # [<ru>] Строка с ключом для доступа к Docker registry (сгенерировано автоматически для вашего токена доступа).
  registryDockerCfg: <YOUR_ACCESS_STRING_IS_HERE>
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
      publishAPI:
        enable: true
        https:
          mode: Global
---
# [<en>] Section containing the parameters of the cloud provider.
# [<en>] https://deckhouse.io/documentation/v1/modules/030-cloud-provider-aws/cluster_configuration.html
# [<ru>] Секция, описывающая параметры облачного провайдера.
# [<ru>] https://deckhouse.io/documentation/v1/modules/030-cloud-provider-aws/cluster_configuration.html
apiVersion: deckhouse.io/v1
kind: GCPClusterConfiguration
layout: WithoutNAT
# [<en>] GCP access parameters.
# [<ru>] Параметры доступа к облаку GCP.
provider:
  # [<en>] Example of serviceAccountJSON:
  # [<ru>] Пример заполнения serviceAccountJSON:
  # serviceAccountJSON: |
  #    {
  #      "type": "service_account",
  #      "project_id": "somproject-sandbox",
  #      "private_key_id": "***",
  #      "private_key": "***",
  #      "client_email": "mail@somemail.com",
  #      "client_id": "<client_id>",
  #      "auth_uri": "https://accounts.google.com/o/oauth2/auth",
  #      "token_uri": "https://oauth2.googleapis.com/token",
  #      "auth_provider_x509_cert_url": "https://www.googleapis.com/oauth2/v1/certs",
  #      "client_x509_cert_url": "https://www.googleapis.com/robot/v1/metadata/x509/somproject-sandbox.gserviceaccount.com"
  #    }
  serviceAccountJSON: *!CHANGE_ServiceAccountJSON*
  # [<en>] Cluster region.
  # [<ru>] Регион привязки кластера.
  # [<en>] You might consider changing this.
  # [<ru>] Возможно, захотите изменить.
  region: europe-west3
# [<en>] List of labels to attach to cluster resources.
# [<ru>] Список меток ресурсов кластера, используются для идентификации в качестве префиксов ресурсов.
labels:
  kube: d8-demo
masterNodeGroup:
  replicas: 1
  instanceClass:
    # [<en>] Type of the instance.
    # [<ru>] Тип используемого инстанса.
    # [<en>] You might consider changing this.
    # [<ru>] Возможно, захотите изменить.
    machineType: n1-standard-4
    # [<en>] Image ID.
    # [<ru>] Используемый образ.
    # [<en>] You might consider changing this.
    # [<ru>] Возможно, захотите изменить.
    image: projects/ubuntu-os-cloud/global/images/ubuntu-2204-jammy-v20221206
    disableExternalIP: false
# [<en>] A subnet to use for cluster nodes.
# [<ru>] Адресное пространство узлов кластера.
subnetworkCIDR: 10.0.0.0/24
# [<en>] Public SSH key for accessing cloud nodes.
# [<en>] This key will be added to the user on created nodes (the user name depends on the image used).
# [<ru>] Публичная часть SSH-ключа для доступа к узлам облака.
# [<ru>] Этот ключ будет добавлен пользователю на созданных узлах (имя пользователя зависит от используемого образа).
sshKey: *!CHANGE_SSH_KEY*
