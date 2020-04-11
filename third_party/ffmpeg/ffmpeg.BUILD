load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

AVLIBS = [
    "avcodec",
    "avdevice",
    "avfilter",
    "avformat",
    "avutil",
    "swresample",
    "swscale",
]

[
    cc_library(
        name = avlib,
        srcs = glob(["lib/x86_64-linux-gnu/lib{}.so*".format(avlib)]),
        hdrs = glob(["include/x86_64-linux-gnu/lib{}/*.h".format(avlib)]),
        includes = ["include/x86_64-linux-gnu"],
    )
    for avlib in AVLIBS
]

cc_library(
    name = "ffmpeg",
    deps = [
        ":{}".format(avlib)
        for avlib in AVLIBS
    ],
)
