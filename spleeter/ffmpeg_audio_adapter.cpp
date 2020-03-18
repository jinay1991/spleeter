///
/// @file
/// @brief Contains definitions for FFMPEG Audio Adapter class methods
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/ffmpeg_audio_adapter.h"
#include "spleeter/logging/logging.h"

namespace spleeter
{
namespace internal
{
void PrintAudioFrameInfo(const AVCodecContext* codec_ctx, const AVFrame* frame)
{
    // See the following to know what data type (unsigned char, short, float, etc) to use to access the audio data:
    // http://ffmpeg.org/doxygen/trunk/samplefmt_8h.html#af9a51ca15301871723577c730b5865c5
    LOG(DEBUG) << "Audio frame info:\n"
               << "  Frame number: " << codec_ctx->frame_number << "\n"
               << "  Frame size: " << codec_ctx->frame_size << "\n"
               << "  Frame data: " << frame->data << "\n"
               << "  Sample count: " << frame->nb_samples << '\n'
               << "  Channel count: " << codec_ctx->channels << '\n'
               << "  Format: " << av_get_sample_fmt_name(codec_ctx->sample_fmt) << '\n'
               << "  Bytes per sample: " << av_get_bytes_per_sample(codec_ctx->sample_fmt) << '\n'
               << "  Is planar? " << av_sample_fmt_is_planar(codec_ctx->sample_fmt) << '\n';
}
}  // namespace internal

FfmpegAudioAdapter::FfmpegAudioAdapter() { av_register_all(); }

FfmpegAudioAdapter::~FfmpegAudioAdapter() {}

/// @ref https://ffmpeg.org/doxygen/trunk/decode_audio_8c-example.html
std::pair<Waveform, std::int32_t> FfmpegAudioAdapter::Load(const std::string& path, const double offset,
                                                           const double duration, const std::int32_t& sample_rate)
{
    AVPacket* packet = av_packet_alloc();
    ASSERT_CHECK(packet) << "Could not allocate packet";

    AVFrame* frame = av_frame_alloc();
    ASSERT_CHECK(frame) << "Could not allocate frame";

    AVFormatContext* format_context = avformat_alloc_context();
    ASSERT_CHECK(format_context) << "Could not allocate format context";

    auto ret = avformat_open_input(&format_context, path.c_str(), nullptr, nullptr);
    ASSERT_CHECK_EQ(0, ret) << "Could not open file " << path;

    ret = avformat_find_stream_info(format_context, nullptr);
    ASSERT_CHECK_EQ(0, ret) << "Could not retrieve stream info from file " << path;

    // Find the audio stream
    AVCodec* audio_codec{nullptr};
    int stream_index = av_find_best_stream(format_context, AVMEDIA_TYPE_AUDIO, -1, -1, &audio_codec, 0);
    ASSERT_CHECK_EQ(0, ret) << "Could not find any audio stream in the file " << path;

    AVStream* audio_stream = format_context->streams[stream_index];
    AVCodecContext* audio_codec_context = audio_stream->codec;
    audio_codec_context->codec = audio_codec;

    ret = avcodec_open2(audio_codec_context, audio_codec_context->codec, NULL);
    ASSERT_CHECK(ret == 0) << "Couldn't open the context with the decoder";

    LOG(INFO) << "Stream has " << audio_codec_context->channels << " channels and a sample rate of "
              << audio_codec_context->sample_rate << "Hz";
    LOG(INFO) << "Data is in the format " << av_get_sample_fmt_name(audio_codec_context->sample_fmt);

    av_init_packet(packet);

    // Read the packets in a loop
    while (av_read_frame(format_context, packet) == 0)
    {
        ret = avcodec_decode_audio4(audio_codec_context, frame, nullptr, packet);
        ASSERT_CHECK_EQ(0, ret) << "Could not decode frame";
    }

    av_free(frame);
    avcodec_close(audio_codec_context);
    avformat_close_input(&format_context);

    Waveform data{};

    LOG(DEBUG) << "Decoded Waveform: \n"
               << " (+) size: " << data[0].size() << "\n"
               << " (+) sample_rate: " << audio_codec_context->sample_rate << "\n";

    return std::make_pair(data, audio_codec_context->sample_rate);
}

/// @ref https://ffmpeg.org/doxygen/trunk/encode_audio_8c-example.html
void FfmpegAudioAdapter::Save(const std::string& path, const Waveform& data, const std::int32_t& sample_rate,
                              const std::string& codec, const std::int32_t& bitrate)
{
    AVCodec* audio_codec = avcodec_find_encoder(AV_CODEC_ID_MP3);
    ASSERT_CHECK(audio_codec) << "Could not find codec {" << codec << "}";

    AVCodecContext* audio_codec_context = avcodec_alloc_context3(audio_codec);
    ASSERT_CHECK(audio_codec_context) << "Could not allocate context";

    audio_codec_context->bit_rate = bitrate;
    audio_codec_context->sample_rate = sample_rate;
    audio_codec_context->sample_fmt = AV_SAMPLE_FMT_S16P;
    audio_codec_context->channels = 2;
    audio_codec_context->channel_layout = 2;

    auto ret = avcodec_open2(audio_codec_context, audio_codec, NULL);
    ASSERT_CHECK_EQ(0, ret) << "Couldn't open the context with the audio codec";

    /// @todo Encode provided Waveform
    AVPacket* packet = av_packet_alloc();
    ASSERT_CHECK(packet) << "Could not allocate packet";

    AVFrame* frame = av_frame_alloc();
    ASSERT_CHECK(frame) << "Could not allocate frame";

    frame->nb_samples = audio_codec_context->frame_size;
    frame->format = audio_codec_context->sample_fmt;
    frame->channel_layout = audio_codec_context->channel_layout;

    ret = av_frame_get_buffer(frame, 0);
    ASSERT_CHECK_EQ(0, ret) << "Could not allocate data buffers to frame";

    std::copy(data[0].begin(), data[0].end(), frame->data[0]);
    std::copy(data[1].begin(), data[1].end(), frame->data[1]);

    ret = avcodec_send_frame(audio_codec_context, frame);
    ASSERT_CHECK_EQ(0, ret) << "Could not send frame to encoder";

    auto fptr = fopen(path.c_str(), "wb");
    ASSERT_CHECK(fptr) << "Could not create/open file {" << path << "}";

    while (ret > 0)
    {
        ret = avcodec_receive_packet(audio_codec_context, packet);
        ASSERT_CHECK_EQ(0, ret) << "Could not encode frame";

        fwrite(packet->data, 1, packet->size, fptr);
        av_packet_unref(packet);
    }

    fclose(fptr);
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_close(audio_codec_context);
    avcodec_free_context(&audio_codec_context);

    LOG(DEBUG) << "Encoded Waveform: \n"
               << " (+) size: " << data[0].size() << "x" << data[1].size() << "\n"
               << " (+) sample_rate: " << audio_codec_context->sample_rate << "\n"
               << " (+) path: " << path;
}
}  // namespace spleeter
