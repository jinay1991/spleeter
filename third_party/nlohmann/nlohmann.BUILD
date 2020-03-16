load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "json",
    hdrs = ["single_include/nlohmann/json.hpp"],
    copts = ["-std=c++14"],
    includes = ["single_include"],
)
