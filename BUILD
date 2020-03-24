load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

# Export Headers
exports_files(glob([
    "spleeter/**/*.h",
]))

filegroup(
    name = "spleeter-dev",
    testonly = True,
    srcs = glob([
        "**/*.h",
    ]) + [
        "//spleeter",
        "//spleeter/test:unit_tests",
        "//spleeter/test:component_tests",
        "//spleeter/audio/test:unit_tests",
        "//spleeter/inference_engine/test:unit_tests",
        "//spleeter/inference_engine",
        "//spleeter/audio",
        "//spleeter/logging/test:unit_tests",
        "//spleeter/logging",
        "//spleeter/argument_parser/test:unit_tests",
        "//spleeter/argument_parser",
    ],
)

pkg_tar(
    name = "spleeter",
    testonly = True,
    srcs = glob([
        "**/*.h",
    ]) + [
        "@audio_example//file",
        "//:spleeter-dev",
        "//application:spleeter",
    ],
    extension = "tar.gz",
    package_dir = "/spleeter",
    strip_prefix = "/",
    tags = ["manual"],
)
