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
