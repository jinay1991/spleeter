load("@rules_cc//cc:defs.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "unit_test",
    srcs = glob(["*.cpp"]),
    data = [
        "@audio_example//file",
    ],
    linkstatic = True,
    deps = [
        "//spleeter/audio",
        "@googletest//:gtest_main",
    ],
)
