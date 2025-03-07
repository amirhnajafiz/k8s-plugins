resource "helm_release" "coredns" {
  name       = "coredns"
  repository = "https://github.com/coredns/helm"
  chart      = "charts/coredns"
  namespace  = "kube-system"

  set {
    name  = "isClusterService"
    value = "true"
  }
}
