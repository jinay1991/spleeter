load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

cc_library(
    name = "inference_engine",
    srcs = [
        "inference_engine_strategy.cpp",
        "null_inference_engine.cpp",
        "tf_inference_engine.cpp",
        "tflite_inference_engine.cpp",
    ],
    hdrs = [
        "i_inference_engine.h",
        "inference_engine_strategy.h",
        "null_inference_engine.h",
        "tf_inference_engine.h",
        "tflite_inference_engine.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//spleeter/datatypes",
        "//spleeter/logging",
        "@tensorflow",
        "@tensorflowlite",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = ["test/inference_engine_tests.cpp"],
    data = [
        "@audio_example//file",
        "@models//:5stems",
    ],
    linkstatic = True,
    tags = ["unit"],
    visibility = ["//visibility:public"],
    deps = [
        ":inference_engine",
        "//spleeter/audio",
        "//spleeter/logging",
        "@googletest//:gtest_main",
    ],
)

pkg_tar(
    name = "includes",
    srcs = glob(["*.h"]),
    mode = "0644",
    package_dir = "spleeter/inference_engine",
    tags = ["manual"],
    visibility = ["//visibility:public"],
)
