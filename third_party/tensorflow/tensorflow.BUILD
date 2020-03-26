load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "tensorflow",
    srcs = glob(["lib/libtensorflow_cc.so*"]),
    hdrs = glob([
        "include/**/*.h",
        "include/**/*",
    ]),
    copts = [
        "-std=c++14",
    ],
    includes = [
        "include",
        "include/third_party/eigen3",
    ],
    deps = [
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:span",
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//:protobuf_headers",
        "@eigen",
    ],
)

pkg_tar(
    name = "tensorflow-lib",
    srcs = glob(["lib/libtensorflow_cc.so*"]),
    mode = "0644",
    package_dir = "tensorflow/lib",
    tags = ["manual"],
)

pkg_tar(
    name = "tensorflow-includes",
    srcs = glob([
        "include/**/*.h",
        "include/**/*",
    ]),
    mode = "0644",
    package_dir = "tensorflow/include",
    tags = ["manual"],
)
