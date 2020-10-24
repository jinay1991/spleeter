load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def models():
    """ Download Models """
    if "spleeter_2stems" not in native.existing_rules():
        http_archive(
            name = "spleeter_2stems",
            build_file = "//third_party/models:models.BUILD",
            sha256 = "f3a90b39dd2874269e8b05a48a86745df897b848c61f3958efc80a39152bd692",
            url = "https://github.com/deezer/spleeter/releases/download/v1.4.0/2stems.tar.gz",
        )

    if "spleeter_4stems" not in native.existing_rules():
        http_archive(
            name = "spleeter_4stems",
            build_file = "//third_party/models:models.BUILD",
            sha256 = "3adb4a50ad4eb18c7c4d65fcf4cf2367a07d48408a5eb7d03cd20067429dfaa8",
            url = "https://github.com/deezer/spleeter/releases/download/v1.4.0/4stems.tar.gz",
        )

    if "spleeter_5stems" not in native.existing_rules():
        http_archive(
            name = "spleeter_5stems",
            build_file = "//third_party/models:models.BUILD",
            sha256 = "25a1e87eb5f75cc72a4d2d5467a0a50ac75f05611f877c278793742513cc7218",
            url = "https://github.com/deezer/spleeter/releases/download/v1.4.0/5stems.tar.gz",
        )

    if "models_1.14" not in native.existing_rules():
        http_archive(
            name = "models_1.14",
            build_file = "//third_party/models:models.BUILD",
            strip_prefix = "exported",
            sha256 = "7d78053099cd8b6d6afabee6bb17aa85f34346e78155b6f6691f4b6568a43ce2",
            url = "https://github.com/gvne/spleeterpp/releases/download/tf_cc-1.14.0/spleeter-models.zip",
        )

    if "models" not in native.existing_rules():
        http_archive(
            name = "models",
            build_file = "//third_party/models:models.BUILD",
            sha256 = "edfdbdb9b830698fc859afbb9f1e5d8b2a7da1ec33ed1ccc288e9a57ef0c4ca2",
            url = "https://github.com/jinay1991/spleeter/releases/download/v2.3/models.zip",
        )
