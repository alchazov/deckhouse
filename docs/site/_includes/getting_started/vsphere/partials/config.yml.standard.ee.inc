# [<en>] General cluster parameters.
# [<en>] https://deckhouse.io/documentation/v1/installing/configuration.html#clusterconfiguration
# [<ru>] Секция с общими параметрами кластера.
# [<ru>] https://deckhouse.ru/documentation/v1/installing/configuration.html#clusterconfiguration
apiVersion: deckhouse.io/v1
kind: ClusterConfiguration
clusterType: Cloud
cloud:
  provider: vSphere
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
# [<en>] https://deckhouse.io/documentation/v1/modules/030-cloud-provider-vsphere/cluster_configuration.html
# [<ru>] Секция, описывающая параметры облачного провайдера.
# [<ru>] https://deckhouse.io/documentation/v1/modules/030-cloud-provider-vsphere/cluster_configuration.html
apiVersion: deckhouse.io/v1
kind: VsphereClusterConfiguration
layout: Standard
# [<en>] vCenter API access parameters
# [<ru>] параметры доступа к API vCenter
provider:
  server: *!CHANGE_SERVER*
  username: *!CHANGE_USERNAME*
  password: *!CHANGE_PASSWORD*
  # [<ru>] Укажите true, если vCenter использует самоподписанный сертификат,
  # [<ru>] иначе укажите false (или удалите строку с параметром insecure).
  # [<en>] Set to true if vCenter has a self-signed certificate,
  # [<en>] otherwise set false (or delete the string below with the insecure parameter).
  insecure: *!CHANGE_INSECURE*
# [<en>] Path to a Folder in which VirtualMachines will be created.
# [<en>] The folder itself will be created by the Deckhouse Installer.
# [<ru>] Путь до Folder, в котором будут создаваться виртуальные машины.
# [<ru>] Сам Folder будет создан Deckhouse Installer'ом.
vmFolderPath: *!CHANGE_FOLDER*
# [<en>] Region and zone tag category names.
# [<ru>] Имена категорий тэгов регионов и зон.
regionTagCategory: k8s-region
zoneTagCategory: k8s-zone
# [<en>] Region and zone tag names in which cluster will operate.
# [<ru>] Имена тэгов зон и регионов, в которых будет работать кластер.
region: *!CHANGE_REGION_TAG_NAME*
zones:
- *!CHANGE_ZONE_TAG_NAME*
# [<en>] Public SSH key for accessing cloud nodes.
# [<en>] This key will be added to the user on created nodes (the user name depends on the image used).
# [<ru>] Публичная часть SSH-ключа для доступа к узлам облака.
# [<ru>] Этот ключ будет добавлен пользователю на созданных узлах (имя пользователя зависит от используемого образа).
sshPublicKey: *!CHANGE_SSH_KEY*
# [<en>] Name of the External Network which has access to the Internet.
# [<en>] IP addresses from the External Network sets as ExternalIP of Node object.
# [<en>] Optional parameter.
# [<ru>] Имя External Network, у которой есть доступ в Интернет.
# [<ru>] IP-адреса из External Network проставляются как ExternalIP объекта Node.
# [<ru>] Опциональный параметр.
externalNetworkNames:
- *!CHANGE_NETWORK_NAME*
# [<en>] Name of the Internal Network that will be used for traffic between nodes.
# [<en>] IP addresses from the Internal Network sets as InternalIP of Node object.
# [<en>] Optional parameter.
# [<ru>] Имя Internal Network, которая будет использоваться для трафика между узлами, в данном примере идентично externalNetworkNames.
# [<ru>] IP-адреса из Internal Network проставляются как InternalIP объекта Node.
# [<ru>] Опциональный параметр.
internalNetworkNames:
- *!CHANGE_NETWORK_NAME*
# [<en>] Address space of the cluster's nodes.
# [<ru>] Адресное пространство узлов кластера.
internalNetworkCIDR: 10.90.0.0/24
masterNodeGroup:
  replicas: 1
  instanceClass:
    numCPUs: 4
    memory: 8192
    rootDiskSize: 50
    # [<en>] The name of the image, taking into account the vCenter folder path.
    # [<en>] Example: "folder/my-ubuntu-packer-image".
    # [<ru>] Имя образа, созданного с учетом папки размещения образа в vCenter.
    # [<ru>] Пример: "folder/my-ubuntu-packer-image".
    template: *!CHANGE_TEMPLATE_NAME*
    datastore: *!CHANGE_DATASTORE_NAME*
    # [<en>] Main network connected to node.
    # [<ru>] Основная сеть подключенная к узлу.
    mainNetwork: *!CHANGE_NETWORK_NAME*
    # [<en>] Additional networks connected to node.
    # [<en>] Optional parameter.
    # [<ru>] Дополнительные сети, подключенные к узлу.
    # [<ru>] Опциональный параметр.
    additionalNetworks:
    - *!CHANGE_NETWORK_NAME*
