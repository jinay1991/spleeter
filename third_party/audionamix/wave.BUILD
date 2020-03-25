load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "wave",
    srcs = glob(
        [
            "src/wave/*.cc",
            "src/wave/header/*.cc",
        ],
        exclude = glob(["src/wave/*_test.cc"]),
    ),
    hdrs = glob([
        "src/wave/*.h",
        "src/wave/header/*.h",
    ]),
    includes = [
        "src",
        "src/wave/header",
    ],
)
