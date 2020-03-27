load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "logging",
    srcs = ["lib/liblogging.so"],
    hdrs = glob(["include/logging/*.h"]),
    copts = ["-std=c++14"],
    includes = ["include"],
)

cc_library(
    name = "argument_parser",
    srcs = ["lib/libargument_parser.so"],
    hdrs = glob(["include/argument_parser/*.h"]),
    copts = ["-std=c++14"],
    includes = ["include"],
    deps = [
        "@spleeter//:logging",
    ],
)

cc_library(
    name = "audio",
    srcs = ["lib/libaudio.so"],
    hdrs = glob([
        "include/audio/*.h",
        "include/data_types/*.h",
    ]),
    copts = ["-std=c++14"],
    includes = ["include"],
    deps = [
        "@audionamix//:wave",
        "@ffmpeg//:avcodec",
        "@ffmpeg//:avformat",
        "@ffmpeg//:avutil",
        "@ffmpeg//:swresample",
        "@spleeter//:logging",
    ],
)

cc_library(
    name = "inference_engine",
    srcs = ["lib/libinference_engine.so"],
    hdrs = glob([
        "include/inference_engine/*.h",
        "include/data_types/*.h",
    ]),
    copts = ["-std=c++14"],
    includes = ["include"],
    deps = [
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
        "@spleeter//:argument_parser",
        "@spleeter//:logging",
        "@tensorflow",
    ],
)

cc_library(
    name = "spleeter",
    srcs = ["lib/libspleeter.so"],
    hdrs = glob([
        "include/*.h",
        "include/data_types/*.h",
        "include/**/*.h",
    ]),
    copts = ["-std=c++14"],
    data = [
        "@audio_example//file",
        "@models//:5stems",
    ],
    includes = ["include"],
    linkopts = [
        "-lstdc++fs",
    ],
    deps = [
        "@spleeter//:argument_parser",
        "@spleeter//:audio",
        "@spleeter//:inference_engine",
        "@spleeter//:logging",
    ],
)
