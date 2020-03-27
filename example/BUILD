load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "spleeter_app",
    srcs = ["spleeter_app.cpp"],
    copts = [
        "-std=c++14",
    ],
    deps = [
        "@spleeter",
    ],
)
