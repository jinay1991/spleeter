///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/ffmpeg_audio_adapter.h"
#include "spleeter/logging/logging.h"

namespace spleeter
{
FfmpegAudioAdapter::FfmpegAudioAdapter() : format_{nullptr}
{
    av_register_all();
    format_ = avformat_alloc_context();
}

FfmpegAudioAdapter::~FfmpegAudioAdapter() { avformat_close_input(&format_); }

std::tuple<Waveform, std::int32_t> FfmpegAudioAdapter::Load(const std::string& path, const double offset,
                                                            const double duration, const std::int32_t& sample_rate)
{
    std::int32_t ret = 0;

    // get format from audio file
    ret = avformat_open_input(&format_, path.c_str(), nullptr, nullptr);
    ASSERT_CHECK(!ret) << "Could not open file " << path;

    ret = avformat_find_stream_info(format_, nullptr);
    ASSERT_CHECK(ret >= 0) << "Could not retrieve stream info from file " << path;

    // Find the audio stream
    AVCodec* codec{nullptr};
    int stream_index = av_find_best_stream(format_, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);
    ASSERT_CHECK(ret >= 0) << "Could not find any audio stream in the file " << path;

    AVStream* audio_stream = format_->streams[stream_index];
    AVCodecContext* codec_ctx = audio_stream->codec;
    codec_ctx->codec = codec;
    

    return std::make_tuple<Waveform, std::int32_t>("", 0);
}

/// @brief Write waveform data to the file denoted by the given path using FFMPEG process.
void FfmpegAudioAdapter::Save(const std::string& path, const std::string& data, const std::int32_t& sample_rate,
                              const std::string& codec)
{
}
}  // namespace spleeter
