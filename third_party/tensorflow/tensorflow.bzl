load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def tensorflow():
    """ Load TensorFlow as Dependency """
    if "bazel_skylib" not in native.existing_rules():
        http_archive(
            name = "bazel_skylib",
            sha256 = "e72747100a8b6002992cc0bf678f6279e71a3fd4a88cab3371ace6c73432be30",
            url = "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.0/bazel-skylib-1.0.0.tar.gz",
        )
    if "io_bazel_rules_closure" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_closure",
            sha256 = "9d359cc1b508082d8ba309ba085da6ecec85e7a4d5bd08f8db9666ee39a85529",
            strip_prefix = "rules_closure-0.9.0",
            url = "https://github.com/bazelbuild/rules_closure/archive/0.9.0.zip",
        )
    if "org_tensorflow" not in native.existing_rules():
        http_archive(
            name = "org_tensorflow",
            sha256 = "4c13e99a2e5fdddc491e003304141f9d9060e11584499061b1b4224e500dc49a",
            strip_prefix = "tensorflow-2.0.0",
            url = "https://github.com/tensorflow/tensorflow/archive/v2.0.0.zip",
        )
