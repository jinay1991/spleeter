load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "spleeter",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    linkopts = [
        "-lstdc++fs",
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

pkg_tar(
    name = "includes",
    srcs = glob(["*.h"]),
    mode = "0644",
    package_dir = "spleeter",
    tags = ["manual"],
)
