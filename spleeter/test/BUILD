load("@rules_cc//cc:defs.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "unit_test",
    srcs = glob(
        ["*.cpp"],
        exclude = ["spleeter_tests.cpp"],
    ),
    copts = [
        "-std=c++14",
        "-Wall",
        "-Werror",
    ],
    data = [
        "@audio_example//file",
    ],
    linkopts = [
        "-lstdc++fs",
    ],
    linkstatic = True,
    deps = [
        "//spleeter",
        "//spleeter/audio",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "component_test",
    srcs = ["spleeter_tests.cpp"],
    copts = [
        "-std=c++14",
        "-Wall",
        "-Werror",
    ],
    data = [
        "@audio_example//file",
    ],
    linkstatic = True,
    deps = [
        "//spleeter",
        "@googletest//:gtest_main",
    ],
)
