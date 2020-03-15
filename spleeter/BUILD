load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "spleeter",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    deps = [
        "//spleeter/argument_parser",
        "//spleeter/logging",
    ],
)
