load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "spleeter",
    srcs = glob(["*.cpp"]),
    copts = [
        "-std=c++14",
        "-Wall",
        "-Werror",
    ],
    data = ["//:testdata"],
    deps = [
        "//spleeter",
    ],
)
