load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visiblity = ["//visibility:public"])

cc_library(
    name = "wave",
    srcs = glob([
        "wave/*.cc",
        "wave/header/*.cc",
    ], exclude = glob(["wave/*_test.cc"])),
    hdrs = glob([
        "wave/*.h",
        "wave/header/*.h",
    ]),
    includes = [
        "wave",
        "wave/header",
    ],
)
