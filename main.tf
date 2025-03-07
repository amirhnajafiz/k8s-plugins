module "coredns" {
  source = "./modules/coredns"
}

module "cilium" {
  source = "./modules/cilium"
}

module "kube-prometheus" {
  source = "./modules/kube-prometheus"
  namespace = "monitoring"
}

module "nfd" {
  source = "./modules/nfd"
  kustomization_url = "https://github.com/kubernetes-sigs/node-feature-discovery/deployment/overlays/default?ref=v0.17.2"
}
