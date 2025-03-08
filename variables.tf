variable "cluster_name" {
  description = "Kubernetes cluster name"
  type        = string
  default     = "kubernetes"
}

variable "kube_config_path" {
  description = "Path to the kubeconfig file"
  type        = string
  default     = "~/.kube/config"
}

variable "namespace" {
  description = "Namespace for components"
  type        = string
  default     = "default"
}

variable "kustomization_url" {
  description = "The URL of kustomization overlay"
  type        = string
}
