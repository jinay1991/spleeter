load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def models():
    if "spleeter_unet" not in native.existing_rules():
        http_archive(
            name = "spleeter_unet",
            build_file = "//third_party/models:models.BUILD",
            sha256 = "25a1e87eb5f75cc72a4d2d5467a0a50ac75f05611f877c278793742513cc7218",
            url = "https://github.com/deezer/spleeter/releases/download/v1.4.0/5stems.tar.gz",
        )
