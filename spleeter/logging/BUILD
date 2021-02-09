load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "logging",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    deps = [
        "@glog",
    ],
)

pkg_tar(
    name = "includes",
    srcs = glob(["*.h"]),
    mode = "0644",
    package_dir = "spleeter/logging",
    tags = ["manual"],
)
