load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "audio",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    copts = [
        "-std=c++14",
        "-Wall",
    ],
    linkstatic = True,
    deps = [
        "//spleeter/data_types",
        "//spleeter/logging",
        "@audionamix//:wave",
        "@ffmpeg//:avcodec",
        "@ffmpeg//:avformat",
        "@ffmpeg//:avutil",
        "@ffmpeg//:swresample",
    ],
)
