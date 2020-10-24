load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def tensorflowlite():
    """ Load TensorFlow Lite as Dependency """
    if "tensorflowlite" not in native.existing_rules():
        http_archive(
            name = "tensorflowlite",
            sha256 = "b8a86faa7c752064a75577f3cfe3f594dd4c44ce863cb3b0ec3bfac096c9e0bf",
            strip_prefix = "libtensorflowlite_cc-2.3.0-multios",
            url = "https://github.com/jinay1991/spleeter/releases/download/v2.3/libtensorflowlite_cc-2.3.0-multios.tar.gz",
        )
