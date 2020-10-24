load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

pkg_tar(
    name = "spleeter-bin",
    srcs = [
        "//application:spleeter",
    ],
    mode = "0647",
    package_dir = "bin",
    tags = ["manual"],
)

pkg_tar(
    name = "spleeter-include",
    srcs = [],
    mode = "0644",
    package_dir = "include",
    tags = ["manual"],
    deps = [
        "//spleeter:includes",
        "//spleeter/argument_parser:includes",
        "//spleeter/audio:includes",
        "//spleeter/datatypes:includes",
        "//spleeter/inference_engine:includes",
        "//spleeter/logging:includes",
    ],
)

pkg_tar(
    name = "spleeter-lib",
    srcs = [
        "//spleeter",
        "//spleeter/argument_parser",
        "//spleeter/audio",
        "//spleeter/datatypes",
        "//spleeter/inference_engine",
        "//spleeter/logging",
    ],
    mode = "0644",
    package_dir = "lib",
    tags = ["manual"],
)

pkg_tar(
    name = "spleeter-data",
    srcs = [
        "@audio_example//file",
    ],
    mode = "0644",
    package_dir = "data",
    tags = ["manual"],
)

pkg_tar(
    name = "spleeter-model",
    srcs = [
        "@model//:5stems",
    ],
    mode = "0644",
    package_dir = "model",
    tags = ["manual"],
)

filegroup(
    name = "spleeter-third_party_files",
    srcs = glob(["third_party/**/*"]),
)

pkg_tar(
    name = "spleeter-third_party",
    srcs = [
        ":spleeter-third_party_files",
    ],
    mode = "0644",
    package_dir = "third_party",
    tags = ["manual"],
)

pkg_tar(
    name = "spleeter-dev",
    testonly = True,
    extension = "tar.gz",
    package_dir = "/spleeter",
    strip_prefix = "/",
    tags = ["manual"],
    deps = [
        ":spleeter-bin",
        ":spleeter-data",
        ":spleeter-include",
        ":spleeter-lib",
        ":spleeter-third_party",
    ],
)
