load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "tensorflow",
    srcs = glob(["lib/libtensorflow_cc.so*"]),
    hdrs = glob([
        "include/**/*.h",
        "include/**/*",
    ]),
    copts = [
        "-std=c++14",
    ],
    includes = [
        "include",
        "include/third_party/eigen3",
    ],
    deps = [
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:span",
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//:protobuf_headers",
        "@eigen",
    ],
)
