load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")

def audio_example():
    if "audio_example" not in native.existing_rules():
        http_file(
            name = "audio_example",
            downloaded_file_path = "audio_example.mp3",
            sha256 = "4b431d535d235bd81b62f816f08c3f1afb6679d2706d84b3b75903b7df909507",
            urls = ["https://raw.githubusercontent.com/deezer/spleeter/master/audio_example.mp3"],
        )

    # if "audio_example" not in native.existing_rules():
    #     http_file(
    #         name = "audio_example",
    #         downloaded_file_path = "audio_example.wav",
    #         sha256 = "0903a9972386da7cb849ce08dac45390eb87c4b412811e7159c91792ed0d9679",
    #         urls = ["https://archive.org/download/test_wav/Untitled3.wav"],
    #     )
