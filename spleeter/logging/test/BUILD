load("@rules_cc//cc:defs.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "unit_test",
    srcs = glob(["*.cpp"]),
    linkstatic = True,
    deps = [
        "//spleeter/logging",
        "@googletest//:gtest_main",
    ],
)
