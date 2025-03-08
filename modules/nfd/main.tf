resource "kubectl_manifest" "nfd_kustomization" {
  manifest = yamldecode(data.http.kustomize_overlay.response_body)
}

data "http" "kustomize_overlay" {
  url = var.kustomization_url
}
