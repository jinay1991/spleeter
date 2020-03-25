load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def eigen():
    if "eigen" not in native.existing_rules():
        http_archive(
            name = "eigen",
            build_file = "//third_party/eigen:eigen.BUILD",
            strip_prefix = "eigen-eigen-323c052e1731",
            sha256 = "65d3aebb5094280869955bcfb41aada2f5194e2d608f930951e810ce4c945c0b",
            url = "https://bitbucket.org/eigen/eigen/get/3.3.7.zip",
        )
