load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")

def audio_example():
    if "audio_example" not in native.existing_rules():
        http_file(
            name = "audio_example",
            downloaded_file_path = "audio_example.wav",
            sha256 = "409f930a5409fe134a16fcb4c12b484794377d7214b14bf42e63957bf17d8f2e",
            urls = ["https://gitlab.com/jinay1991/spleeter/uploads/ded94b0b51d7021328491bac73d6c00a/audio_example.wav"],
        )
