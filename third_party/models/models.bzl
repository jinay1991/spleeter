load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def models():
    """ Download Models """
    if "models" not in native.existing_rules():
        http_archive(
            name = "models",
            build_file = "//third_party/models:models.BUILD",
            strip_prefix = "models",
            sha256 = "e1314aa69ed793e2677689a9cf6f3ac1d2357c80d5fb64769ee7175755ef6a5a",
            url = "https://github.com/jinay1991/spleeter/releases/download/v2.3/models.tar.gz",
        )

    if "2stems" not in native.existing_rules():
        http_archive(
            name = "2stems",
            build_file = "//third_party/models:stems.BUILD",
            strip_prefix = "models/2stems",
            sha256 = "6c2bb7dc3f1c1e162e4567ea4f0f5c20257a0d96c9b3c4e88b363aa83d4cf150",
            url = "https://github.com/jinay1991/spleeter/releases/download/v2.3/2stems.tar.gz",
        )

    if "4stems" not in native.existing_rules():
        http_archive(
            name = "4stems",
            build_file = "//third_party/models:stems.BUILD",
            sha256 = "1edbb653058a9d474370a8579b18f18e9d383965c6749006ded2e1ace0e1af65",
            strip_prefix = "models/4stems",
            url = "https://github.com/jinay1991/spleeter/releases/download/v2.3/4stems.tar.gz",
        )

    if "5stems" not in native.existing_rules():
        http_archive(
            name = "5stems",
            build_file = "//third_party/models:stems.BUILD",
            strip_prefix = "models/5stems",
            sha256 = "667c13427502623ba0a6b3d6ef6b81b9f81b0d13e7e22239c662890b477a8f02",
            url = "https://github.com/jinay1991/spleeter/releases/download/v2.3/5stems.tar.gz",
        )
