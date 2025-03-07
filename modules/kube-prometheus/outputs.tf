output "prometheus_url" {
  description = "Prometheus Web UI"
  value       = "http://kube-prometheus-stack-prometheus.${var.namespace}.svc.cluster.local:9090"
}

output "grafana_url" {
  description = "Grafana Web UI"
  value       = "http://kube-prometheus-stack-grafana.${var.namespace}.svc.cluster.local:3000"
}
