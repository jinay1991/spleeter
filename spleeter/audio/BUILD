load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "audio",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    copts = [
        "-std=c++14",
        "-Wall",
    ],
    deps = [
        "//spleeter/data_types",
        "//spleeter/logging",
        "@audionamix//:wave",
        "@ffmpeg",
    ],
)

cc_binary(
    name = "transcode",
    srcs = ["transcode.c"],
    data = [
        "@audio_example//file",
    ],
    deps = [
        "@ffmpeg",
    ],
)

pkg_tar(
    name = "includes",
    srcs = glob(["*.h"]),
    mode = "0644",
    package_dir = "spleeter/audio",
    tags = ["manual"],
)
