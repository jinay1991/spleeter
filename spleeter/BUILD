load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "spleeter",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    copts = [
        "-std=c++14",
        "-Wall",
        #"-Werror",
        #"-Wno-error=write-strings",
    ],
    deps = [
        "//spleeter/argument_parser",
        "//spleeter/audio",
        "//spleeter/data_types",
        "//spleeter/inference_engine",
        "//spleeter/logging",
        "@nlohmann//:json",
    ],
)

# Export Headers
exports_files(glob([
    "**/*.h",
]))

filegroup(
    name = "spleeter-dev",
    testonly = True,
    srcs = glob([
        "**/*.h",
    ]) + [
        "//spleeter",
        "//spleeter/test:unit_tests",
    ],
)
