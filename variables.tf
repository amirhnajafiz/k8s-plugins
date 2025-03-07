variable "cluster_name" {
  description = "Kubernetes cluster name"
  type        = string
}

variable "kube_config_path" {
  default = "~/.kube/config"
}
