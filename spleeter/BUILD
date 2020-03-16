load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "spleeter",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    copts = [
        "-std=c++14",
        "-Wall",
        #"-Werror",
        #"-Wno-error=write-strings",
    ],
    linkopts = [
        "-lm",
    ],
    deps = [
        "//spleeter/argument_parser",
        "//spleeter/logging",
        "@ffmpeg//:avcodec",
        "@ffmpeg//:avformat",
        "@ffmpeg//:avutil",
        "@ffmpeg//:swresample",
        "@nlohmann//:json",
    ],
)
