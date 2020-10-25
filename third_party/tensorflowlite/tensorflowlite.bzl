load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def tensorflowlite():
    """ Load TensorFlow Lite as Dependency """
    if "tensorflowlite" not in native.existing_rules():
        http_archive(
            name = "tensorflowlite",
            sha256 = "d7c256c5678e85e47824308d63409c4cfada4f326754094f2a1a501ce650812e",
            strip_prefix = "libtensorflowlite_cc-2.3.0-linux",
            url = "https://github.com/jinay1991/spleeter/releases/download/v2.3/libtensorflowlite_cc-2.3.0-linux.tar.gz",
        )
