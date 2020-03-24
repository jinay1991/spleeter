load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "inference_engine",
    srcs = glob(
        ["*.cpp"],
        exclude = ["tflite_inference_engine.cpp"],
    ),
    hdrs = glob(
        ["*.h"],
        exclude = ["tflite_inference_engine.h"],
    ),
    copts = [
        "-std=c++14",
        "-Wall",
        "-Werror",
    ],
    data = [
        "@spleeter_saved_models//:5stems",
    ],
    deps = [
        "//spleeter/argument_parser",
        "//spleeter/data_types",
        "//spleeter/logging",
        "@tensorflow_cc",
        # "@org_tensorflow//tensorflow/cc:client_session",
        # "@org_tensorflow//tensorflow/cc/saved_model:loader",
        # "@org_tensorflow//tensorflow/lite:framework",
        # "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
    ],
)
