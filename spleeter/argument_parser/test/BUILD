load("@rules_cc//cc:defs.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "unit_tests",
    srcs = glob(["*.cpp"]),
    copts = [
        "-std=c++14",
        "-Wall",
        "-Werror",
        "-Wno-error=write-strings",
    ],
    deps = [
        "//spleeter/argument_parser",
        "//spleeter/logging",
        "@googletest//:gtest_main",
    ],
)
