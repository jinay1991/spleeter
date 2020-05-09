load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "tflite_inference_engine",
    srcs = [
        "tflite_inference_engine.cpp",
    ],
    hdrs = [
        "i_inference_engine.h",
        "tflite_inference_engine.h",
    ],
    deps = [
        "//spleeter/data_types",
        "//spleeter/logging",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
    ],
)

cc_library(
    name = "tf_inference_engine",
    srcs = [
        "tf_inference_engine.cpp",
    ],
    hdrs = [
        "i_inference_engine.h",
        "tf_inference_engine.h",
    ],
    deps = [
        "//spleeter/data_types",
        "//spleeter/logging",
        "@tensorflow",
    ],
)

cc_library(
    name = "torch_inference_engine",
    srcs = [
        "torch_inference_engine.cpp",
    ],
    hdrs = [
        "i_inference_engine.h",
        "torch_inference_engine.h",
    ],
    deps = [
        "//spleeter/data_types",
        "//spleeter/logging",
        "@torch",
    ],
)

cc_library(
    name = "inference_engine",
    srcs = [
        "inference_engine.cpp",
    ],
    hdrs = [
        "i_inference_engine.h",
        "inference_engine.h",
    ],
    data = [
        "@models//:2stems",
        "@models//:4stems",
        "@models//:5stems",
    ],
    deps = [
        ":tf_inference_engine",
        ":tflite_inference_engine",
        ":torch_inference_engine",
        "//spleeter/argument_parser",
        "//spleeter/data_types",
        "//spleeter/logging",
    ],
)

pkg_tar(
    name = "includes",
    srcs = glob(["*.h"]),
    mode = "0644",
    package_dir = "spleeter/inference_engine",
    tags = ["manual"],
)
