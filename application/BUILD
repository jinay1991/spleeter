load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "spleeter",
    srcs = glob(["*.cpp"]),
    copts = [
        "-std=c++14",
        "-Wall",
        "-Werror",
    ],
    data = ["@audio_example//file"],
    deps = [
        "//spleeter",
    ],
)
