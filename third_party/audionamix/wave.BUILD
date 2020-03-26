load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

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

pkg_tar(
    name = "wave-lib",
    srcs = [":wave"],
    mode = "0644",
    package_dir = "wave/lib",
    tags = ["manual"],
)

pkg_tar(
    name = "wave-includes",
    srcs = glob([
        "src/wave/*.h",
        "src/wave/header/*.h",
    ]),
    mode = "0644",
    package_dir = "wave/include",
    tags = ["manual"],
)
