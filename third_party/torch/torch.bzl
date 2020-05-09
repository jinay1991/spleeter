load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def torch():
    """ Load PyTorch (C++) Library dependencies """
    if "torch" not in native.existing_rules():
        http_archive(
            name = "torch",
            build_file = "//third_party/torch:torch.BUILD",
            sha256 = "3e438237a08099a4bf014335cd0da88708da3a1678aec12a46c67305792b5fa4",
            url = "https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.5.0%2Bcpu.zip",
            strip_prefix = "libtorch",
        )
