load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "argument_parser",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    copts = ["-Wno-error=writable-strings"],
    deps = [
        "//spleeter/logging",
    ],
)
