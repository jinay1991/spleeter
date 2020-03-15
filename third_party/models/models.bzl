load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def models():
    if "mobilenet_v2_1.0_224_quant" not in native.existing_rules():
        http_archive(
            name = "mobilenet_v2_1.0_224_quant",
            build_file = "//third_party/models:models.BUILD",
            sha256 = "d6a04d780f76f656c902413be432eb349ec4a458240e3739119eb44977f77a79",
            url = "https://storage.googleapis.com/download.tensorflow.org/models/tflite_11_05_08/mobilenet_v2_1.0_224_quant.tgz",
        )
    if "" not in native.existing_rules():
        http_archive(
            name = "mobilenet_v2_1.0_224_quant",
            build_file = "//third_party/models:models.BUILD",
            sha256 = "d6a04d780f76f656c902413be432eb349ec4a458240e3739119eb44977f77a79",
            url = "https://storage.googleapis.com/download.tensorflow.org/models/tflite_11_05_08/mobilenet_v2_1.0_224_quant.tgz",
        )
