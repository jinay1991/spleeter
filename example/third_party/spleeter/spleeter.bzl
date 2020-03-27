load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def spleeter():
    """ Load spleeter as Dependency """
    if "spleeter" not in native.existing_rules():
        http_archive(
            name = "spleeter",
            strip_prefix = "spleeter",
            build_file = "//third_party/spleeter:spleeter.BUILD",
            sha256 = "0fdc07d60199cc4b326a5f798629603cab716397989f2a5c70c546b5f4c169b2",
            url = "https://github.com/jinay1991/spleeter/releases/download/v1.4/spleeter-dev.tar.gz",
        )
