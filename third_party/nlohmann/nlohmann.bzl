load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def nlohmann():
    if "nlohmann" not in native.existing_rules():
        http_archive(
            name = "nlohmann",
            build_file = "//third_party/nlohmann:nlohmann.BUILD",
            url = "https://github.com/nlohmann/json/releases/download/v3.7.3/include.zip",
            sha256 = "87b5884741427220d3a33df1363ae0e8b898099fbc59f1c451113f6732891014",
        )
