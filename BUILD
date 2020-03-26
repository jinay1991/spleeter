load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

# Export Headers
exports_files(glob([
    "spleeter/**/*.h",
]))

pkg_tar(
    name = "spleeter",
    testonly = True,
    srcs = glob([
        "**/*.h",
    ]) + [
        "//application:spleeter",
        "//spleeter",
        "//spleeter/argument_parser",
        "//spleeter/argument_parser/test:unit_test",
        "//spleeter/audio",
        "//spleeter/audio/test:unit_test",
        "//spleeter/inference_engine",
        "//spleeter/inference_engine/test:unit_test",
        "//spleeter/logging",
        "//spleeter/logging/test:unit_test",
        "//spleeter/test:component_test",
        "//spleeter/test:unit_test",
        "@audio_example//file",
        "@models//:5stems",
        "@tensorflow",
    ],
    extension = "tar.gz",
    package_dir = "/spleeter",
    strip_prefix = "/",
    tags = ["manual"],
)
