load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "tensorflow_cc",
    srcs = glob(["**/*.so*"]),
    hdrs = glob(["**/*.h"]),
    includes = [
        "include",
        "third_party/eigen3/unsupported",
    ],
    deps = [
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_protobuf",
    ],
)
