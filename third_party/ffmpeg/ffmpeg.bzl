def ffmpeg():
    if "ffmpeg" not in native.existing_rules():
        native.new_local_repository(
            name = "ffmpeg",
            build_file = "//third_party/ffmpeg:ffmpeg.BUILD",
            path = "/usr/",
        )
