package(default_visibility = ["//visibility:public"])

cc_library(
    name = "eigen",
    hdrs = glob(
        ["Eigen/**"],
        exclude = [
            "Eigen/src/Core/arch/AVX/PacketMathGoogleTest.cc",
        ],
    ),
    copts = [
        "-std=c++14",
    ],
    includes = ["."],
    visibility = ["//visibility:public"],
)
