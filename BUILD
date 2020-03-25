load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

# Export Headers
exports_files(glob([
    "spleeter/**/*.h",
]))

pkg_tar(
    name = "spleeter-headers",
    testonly = True,
    srcs = glob([
        "**/*.h",
    ]),
    extension = "tar.gz",
    package_dir = "/spleeter/include",
    strip_prefix = "/",
    tags = ["manual"],
)

pkg_tar(
    name = "spleeter-libs",
    testonly = True,
    srcs = [
        "//spleeter",
        "//spleeter/argument_parser",
        "//spleeter/audio",
        "//spleeter/inference_engine",
        "//spleeter/logging",
        "@audio_example//file",
    ],
    extension = "tar.gz",
    package_dir = "/spleeter/lib",
    strip_prefix = "/",
    tags = ["manual"],
)

pkg_tar(
    name = "spleeter-bins",
    testonly = True,
    srcs = [
        "//application:spleeter",
    ],
    extension = "tar.gz",
    package_dir = "/spleeter/bin",
    strip_prefix = "/",
    tags = ["manual"],
)

pkg_tar(
    name = "spleeter-tests",
    testonly = True,
    srcs = [
        "//spleeter/argument_parser/test:unit_test",
        "//spleeter/audio/test:unit_test",
        "//spleeter/inference_engine/test:unit_test",
        "//spleeter/logging/test:unit_test",
        "//spleeter/test:component_test",
        "//spleeter/test:unit_test",
    ],
    extension = "tar.gz",
    package_dir = "/spleeter/test",
    strip_prefix = "/",
    tags = ["manual"],
)

pkg_tar(
    name = "spleeter-third_party",
    testonly = True,
    srcs = [
        "@audio_example//file",
        "@models//:2stems",
        "@models//:4stems",
        "@models//:5stems",
        "@tensorflow",
    ],
    extension = "tar.gz",
    package_dir = "/spleeter/third_party",
    strip_prefix = "/",
    tags = ["manual"],
)

pkg_tar(
    name = "spleeter",
    testonly = True,
    srcs = [
        "//:spleeter-bins",
        "//:spleeter-headers",
        "//:spleeter-libs",
        "//:spleeter-tests",
        "//:spleeter-third_party",
    ],
    extension = "tar.gz",
    package_dir = "/spleeter",
    strip_prefix = "/",
    tags = ["manual"],
)
