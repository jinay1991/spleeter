load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def audionamix():
    """ Load audionamix as Dependency """
    if "audionamix" not in native.existing_rules():
        http_archive(
            name = "audionamix",
            strip_prefix = "wave-0.8.2a",
            build_file = "//third_party/audionamix:wave.BUILD",
            sha256 = "b03fb60abf053107864e69c530c8aad5866ccb9de7d216b18ce347cf2b644dd6",
            url = "https://github.com/audionamix/wave/archive/v0.8.2a.zip",
        )
