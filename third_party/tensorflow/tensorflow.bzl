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
            # url = "https://github.com/protocolbuffers/protobuf/releases/download/v3.9.2/protobuf-cpp-3.9.2.zip",
            url = "https://github.com/protocolbuffers/protobuf/archive/310ba5ee72661c081129eb878c1bbcec936b20f0.tar.gz",
            # sha256 = "a79f9c5ac940f4477d18b2ecddb31271643eb8ac6728d0c6b10d95b435909c41",
            sha256 = "b9e92f9af8819bbbc514e2902aec860415b70209f31dfc8c4fa72515a5df9d59",
            # strip_prefix = "protobuf-3.9.2",
            strip_prefix = "protobuf-310ba5ee72661c081129eb878c1bbcec936b20f0",
        )

    if "tensorflow_cc" not in native.existing_rules():
        http_archive(
            name = "tensorflow_cc",
            build_file = "//third_party/tensorflow:tensorflow_cc.BUILD",
            sha256 = "68404e0bc997373e0c63f43ee04479cbe68dcaf3ae0a7a3022a81591ff3a3364",
            url = "https://github.com/jinay1991/spleeter/releases/download/v0.1/libtensorflow_cc-linux.tar.gz",
        )
