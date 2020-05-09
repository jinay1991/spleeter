load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "caffe2",
    srcs = ["lib/libtorch.so"],
    includes = [
        "include",
        "include/torch/csrc/api/include",
    ],
    deps = [
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//:protobuf_headers",
    ],
)

cc_library(
    name = "gomp",
    srcs = glob(["lib/libgomp-*"]),
    includes = [
        "include",
        "include/torch/csrc/api/include",
    ],
    linkopts = [
        "-lpthread",
    ],
)

cc_library(
    name = "c10",
    srcs = [
        "lib/libc10.so",
    ],
    includes = [
        "include",
        "include/torch/csrc/api/include",
    ],
    linkopts = [
        "-ldl",
        "-lpthread",
    ],
    deps = [
        ":gomp",
    ],
)

cc_library(
    name = "torch",
    srcs = [
        "lib/libXNNPACK.a",
        "lib/libclog.a",
        "lib/libcpuinfo.a",
        "lib/libnnpack.a",
        "lib/libpytorch_qnnpack.a",
        "lib/libtorch_cpu.so",
    ],
    hdrs = glob([
        "include/**/*.h",
        "include/**/*",
    ]),
    copts = [
        "-std=c++14",
    ],
    includes = [
        "include",
        "include/torch/csrc/api/include",
    ],
    deps = [
        ":c10",
        ":caffe2",
    ],
)
