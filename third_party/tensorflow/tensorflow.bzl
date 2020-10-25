load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def tensorflow():
    """ Load TensorFlow as Dependency """
    if "tensorflow" not in native.existing_rules():
        http_archive(
            name = "tensorflow",
            sha256 = "74d79a3c7b7c8cd23f08a8695bc8936deeae5223dc920c863bccb684171a5e7a",
            strip_prefix = "libtensorflow_cc-2.3.0-linux",
            url = "https://github.com/jinay1991/spleeter/releases/download/v2.3/libtensorflow_cc-2.3.0-linux.tar.gz",
        )
