load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "eigen",
    hdrs = glob(
        ["Eigen/**"],
        exclude = [
            "Eigen/src/Core/arch/AVX/PacketMathGoogleTest.cc",
        ],
    ),
    copts = [
        "-std=c++14",
    ],
    includes = ["."],
    visibility = ["//visibility:public"],
)

pkg_tar(
    name = "eigen-lib",
    srcs = [":eigen"],
    mode = "0644",
    package_dir = "eigen/lib",
    tags = ["manual"],
)

pkg_tar(
    name = "eigen-includes",
    srcs = glob(
        ["Eigen/**"],
        exclude = [
            "Eigen/src/Core/arch/AVX/PacketMathGoogleTest.cc",
        ],
    ),
    mode = "0644",
    package_dir = "eigen/include",
    tags = ["manual"],
)
