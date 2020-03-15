///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/ffmpeg_audio_adapter.h"
#include "spleeter/logging/logging.h"

namespace spleeter
{
FfmpegAudioAdapter::FfmpegAudioAdapter()
{
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
    avcodec_register_all();
#endif
}

void FfmpegAudioAdapter::Load(const std::string& audio_description, const double offset, const double duration,
                              const std::string& sample_rate)
{
    // auto packet = av_packet_alloc();

    auto codec = avcodec_find_decoder_by_name("mp3");
    ASSERT_CHECK(codec) << "Codec not found!";

    auto parser = av_parser_init(codec->id);
    ASSERT_CHECK(parser) << "Parser not found!";

    auto ctx = avcodec_alloc_context3(codec);
    ASSERT_CHECK(ctx) << "Could not allocate audio codec context!";

    auto ret = avcodec_open2(ctx, codec, nullptr);
    ASSERT_CHECK(ret >= 0) << "Could not open codec!";
}

/// @brief Write waveform data to the file denoted by the given path using FFMPEG process.
void FfmpegAudioAdapter::Save(const std::string& path, const std::string& data, const std::string& sample_rate,
                              const std::string& codec)
{
}
}  // namespace spleeter
