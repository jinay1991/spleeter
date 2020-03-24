load("@rules_cc//cc:defs.bzl", "cc_library")

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
        "@models//:5stems",
    ],
    deps = [
        "//spleeter/argument_parser",
        "//spleeter/data_types",
        "//spleeter/logging",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
        "@tensorflow",
    ],
)
