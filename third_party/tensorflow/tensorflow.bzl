load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def tensorflow():
    """ Load TensorFlow as Dependency """
    if "tensorflow" not in native.existing_rules():
        http_archive(
            name = "tensorflow",
            sha256 = "a8902992b68c19e183279c7a9685b7bc843bf62385f6204d7fffba0967fd75ef",
            strip_prefix = "libtensorflow_cc-2.3.0-multios",
            url = "https://github.com/jinay1991/spleeter/releases/download/v2.3/libtensorflow_cc-2.3.0-multios.tar.gz",
        )
