load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def tensorflowlite():
    """ Load TensorFlow Lite as Dependency """
    if "tensorflowlite" not in native.existing_rules():
        http_archive(
            name = "tensorflowlite",
            sha256 = "09f660b02222f1e6e59287e9ddd9755a7835aaf4cb0e9f46a1efb5af0df5a2c6",
            strip_prefix = "libtensorflowlite_cc-2.3.0-linux",
            url = "https://github.com/jinay1991/spleeter/releases/download/v2.3/libtensorflowlite_cc-2.3.0-linux.tar.gz",
        )
