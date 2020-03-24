load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def tensorflow():
    """ Load TensorFlow as Dependency """
    if "bazel_skylib" not in native.existing_rules():
        http_archive(
            name = "bazel_skylib",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
                "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
            ],
            sha256 = "97e70364e9249702246c0e9444bccdc4b847bed1eb03c5a3ece4f83dfe6abc44",
        )

    if "io_bazel_rules_closure" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_closure",
            sha256 = "9d4b3af0efb86b4f3b1658d12ad485e089587b537bfee0474f2a8549ad1d7f2c",
            strip_prefix = "rules_closure-0.10.0",
            url = "https://github.com/bazelbuild/rules_closure/archive/0.10.0.zip",
        )

    if "org_tensorflow" not in native.existing_rules():
        http_archive(
            name = "org_tensorflow",
            sha256 = "e82f3b94d863e223881678406faa5071b895e1ff928ba18578d2adbbc6b42a4c",
            strip_prefix = "tensorflow-2.1.0",
            url = "https://github.com/tensorflow/tensorflow/archive/v2.1.0.zip",
        )

    if "com_google_protobuf" not in native.existing_rules():
        http_archive(
            name = "com_google_protobuf",
            strip_prefix = "protobuf-3.11.4",
            url = "https://github.com/protocolbuffers/protobuf/releases/download/v3.11.4/protobuf-cpp-3.11.4.tar.gz",
            sha256 = "ba4bbc3e6b58d2ccfe406e616576ef48710a2aae205f463618025fc691549cfe",
        )

    if "tensorflow_cc" not in native.existing_rules():
        http_archive(
            name = "tensorflow_cc",
            strip_prefix = "install",
            build_file = "//third_party/tensorflow:tensorflow_cc.BUILD",
            sha256 = "c2d187a74a4d5e073a641949b4120c3beda523abed9ecfc7cd46a774f3e0d743",
            url = "https://github.com/gvne/spleeterpp/releases/download/tf_cc-1.14.0/tf_cc-1.14.0-linux.zip",
        )
