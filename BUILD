load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

exports_files(glob([
    "data/*.mp3",
]))

filegroup(
    name = "testdata",
    srcs = [
        "data/audio_example.mp3",
    ],
)

pkg_tar(
    name = "spleeter",
    testonly = True,
    srcs = [
        "//:testdata",
        "//application:spleeter",
        "//spleeter:spleeter-dev",
    ],
    extension = "tar.gz",
    package_dir = "/spleeter",
    strip_prefix = "/",
    tags = ["manual"],
)
