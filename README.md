# K8S Plugins

This Terraform project deploys necessary plugins using Helm on a Kubernetes cluster. It sets up Prometheus, Grafana, and Alertmanager for cluster monitoring, cilium for CNI, and CoreDNS.

## Prerequisites

Ensure you have the following installed:

- [Terraform](https://developer.hashicorp.com/terraform/downloads)
- [kubectl](https://kubernetes.io/docs/tasks/tools/)
- [Helm](https://helm.sh/docs/intro/install/)
- A running Kubernetes cluster (e.g., Minikube, EKS, GKE, AKS, etc.)
- A valid kubeconfig file (`~/.kube/config`)

## Deployment Steps

### Initialize Terraform

```sh
terraform init
```

### Apply the Terraform Configuration

```sh
terraform apply -auto-approve
```

## Destroy the Deployment

To remove all deployed resources, run:
```sh
terraform destroy -auto-approve
```

## Plugins

- CoreDNS: Kubernetes DNS deployment
- Cilium: Kubernetes Container Network Interface
- Kube Prometheus: Monitoring stack, including Prometheus, Grafan, and Alert Manager
- Node Feature Discovery: Exporting node metrics
- Nvidia Device Plugin: Using NVIDIA toolkit systemcalls
- DCGM Exporter: Exporting GPU metrics
