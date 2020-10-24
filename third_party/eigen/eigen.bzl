load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

def eigen():
    if "eigen" not in native.existing_rules():
        new_git_repository(
            name = "eigen",
            build_file = "//third_party/eigen:eigen.BUILD",
            remote = "https://gitlab.com/libeigen/eigen.git",
            tag = "3.3.7",
        )
