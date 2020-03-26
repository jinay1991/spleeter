load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "inference_engine",
    srcs = glob(
        ["*.cpp"],
    ),
    hdrs = glob(
        ["*.h"],
    ),
    copts = [
        "-std=c++14",
        "-Wall",
        "-Werror",
    ],
    data = [
        "@models//:2stems",
        "@models//:4stems",
        "@models//:5stems",
    ],
    linkstatic = True,
    deps = [
        "//spleeter/argument_parser",
        "//spleeter/data_types",
        "//spleeter/logging",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
        "@tensorflow",
    ],
)

pkg_tar(
    name = "includes",
    srcs = glob(["*.h"]),
    mode = "0644",
    package_dir = "spleeter/inference_engine",
    tags = ["manual"],
)
