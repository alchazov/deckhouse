global:
  scrape_interval: 5m
  scrape_timeout: 1m
  evaluation_interval: 5m
{{- if or (hasKey .Values.prometheus "madisonAuthKey") (hasKey .Values.prometheus "additionalAlertmanagers") }}
alerting:
  alert_relabel_configs:
  - separator: ;
    regex: prometheus_replica
    replacement: $1
    action: labeldrop
  alertmanagers:
  {{- if hasKey .Values.prometheus "madisonAuthKey" }}
  - kubernetes_sd_configs:
    - role: endpoints
      namespaces:
        names:
        - kube-prometheus
    scheme: http
    path_prefix: /
    timeout: 10s
    relabel_configs:
    - source_labels: [__meta_kubernetes_service_name]
      separator: ;
      regex: madison-proxy
      replacement: $1
      action: keep
    - source_labels: [__meta_kubernetes_endpoint_port_name]
      separator: ;
      regex: http
      replacement: $1
      action: keep
  {{- end }}
  {{- if hasKey .Values.prometheus "additionalAlertmanagers" }}
    {{- range .Values.prometheus.additionalAlertmanagers }}
  - kubernetes_sd_configs:
    - role: endpoints
      namespaces:
        names:
        - {{ .namespace }}
    scheme: http
    path_prefix: {{ .pathPrefix }}
    timeout: 10s
    relabel_configs:
    - source_labels: [__meta_kubernetes_service_name]
      separator: ;
      regex: {{ .name }}
      replacement: $1
      action: keep
    - source_labels: [__meta_kubernetes_pod_container_port_number]
      separator: ;
      regex: {{ .port | quote }}
      replacement: $1
      action: keep
    {{- end }}
  {{- end }}
{{- end }}
scrape_configs:
- job_name: 'federate'
  honor_labels: true
  metrics_path: '/federate'
  scheme: https
  tls_config:
    cert_file: /etc/prometheus/secrets/prometheus-api-client-cert/tls.crt
    key_file: /etc/prometheus/secrets/prometheus-api-client-cert/tls.key
    insecure_skip_verify: true
  params:
    'match[]':
    - '{job=~".+"}'
  static_configs:
  {{- if (include "helm_lib_ha_enabled" .) }}
  - targets: ['prometheus-affinitive.kube-prometheus.svc.{{ .Values.global.discovery.clusterDomain }}.:9090']
  {{- else }}
  - targets: ['prometheus.kube-prometheus.svc.{{ .Values.global.discovery.clusterDomain }}.:9090']
  {{- end }}
