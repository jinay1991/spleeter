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

# cc_library(
#     name = "avformat",
#     srcs = glob(["libavformat.so*"]),
# )

# cc_library(
#     name = "avutil",
#     srcs = glob(["libavutil.so*"]),
# )

# cc_library(
#     name = "avcodec",
#     srcs = glob(["libavcodec.so*"]),
# )

# cc_library(
#     name = "swresample",
#     srcs = glob(["libswresample.so*"]),
# )

# cc_library(
#     name = "avfilter",
#     srcs = glob(["libavfilter.so*"]),
# )

# cc_library(
#     name = "avdevice",
#     srcs = glob(["libavdevice.so*"]),
# )

# cc_library(
#     name = "swscale",
#     srcs = glob(["libswscale.so*"]),
# )
