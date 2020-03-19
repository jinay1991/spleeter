///
/// @file
/// @brief Contains definitions for FFMPEG Audio Adapter class methods
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/ffmpeg_audio_adapter.h"
#include "spleeter/logging/logging.h"

#include <algorithm>
#include <string>

namespace spleeter
{
namespace internal
{
void PrintAudioFrameInfo(const AVCodecContext* coaudio_codec_context, const AVFrame* frame)
{
    // See the following to know what data type (unsigned char, short, float, etc) to use to access the audio data:
    // http://ffmpeg.org/doxygen/trunk/samplefmt_8h.html#af9a51ca15301871723577c730b5865c5
    LOG(DEBUG) << "Audio frame info:\n"
               << "  Frame number: " << coaudio_codec_context->frame_number << "\n"
               << "  Frame size: " << coaudio_codec_context->frame_size << "\n"
               << "  Frame data: " << frame->data << "\n"
               << "  Sample count: " << frame->nb_samples << '\n'
               << "  Channel count: " << coaudio_codec_context->channels << '\n'
               << "  Format: " << av_get_sample_fmt_name(coaudio_codec_context->sample_fmt) << '\n'
               << "  Bytes per sample: " << av_get_bytes_per_sample(coaudio_codec_context->sample_fmt) << '\n'
               << "  Is planar? " << av_sample_fmt_is_planar(coaudio_codec_context->sample_fmt) << '\n';
}

static std::int32_t SelectSampleRate(const AVCodec* codec)
{
    std::int32_t best_samplerate = 0;
    if (!codec->supported_samplerates)
    {
        return 44100;
    }
    const std::int32_t* p = codec->supported_samplerates;
    while (*p)
    {
        if (!best_samplerate || abs(44100 - *p) < abs(44100 - best_samplerate)) best_samplerate = *p;
        p++;
    }
    return best_samplerate;
}

static std::int32_t SelectChannelLayout(const AVCodec* codec)
{
    std::uint64_t best_ch_layout = 0;
    std::int32_t best_nb_channels = 0;
    if (!codec->channel_layouts)
    {
        return AV_CH_LAYOUT_STEREO;
    }
    const std::uint64_t* p = codec->channel_layouts;
    while (*p)
    {
        std::int32_t nb_channels = av_get_channel_layout_nb_channels(*p);
        if (nb_channels > best_nb_channels)
        {
            best_ch_layout = *p;
            best_nb_channels = nb_channels;
        }
        p++;
    }
    return best_ch_layout;
}
static std::string GetFormatFromSampleFormat(AVSampleFormat& sample_fmt)
{
    std::int32_t i;
    struct SampleFormatEntry
    {
        enum AVSampleFormat sample_fmt;
        const char *fmt_be, *fmt_le;
    };
    SampleFormatEntry sample_fmt_entries[] = {
        {AV_SAMPLE_FMT_U8, "u8", "u8"},        {AV_SAMPLE_FMT_S16, "s16be", "s16le"},
        {AV_SAMPLE_FMT_S32, "s32be", "s32le"}, {AV_SAMPLE_FMT_FLT, "f32be", "f32le"},
        {AV_SAMPLE_FMT_DBL, "f64be", "f64le"},
    };
    for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++)
    {
        SampleFormatEntry* entry = &sample_fmt_entries[i];
        if (sample_fmt == entry->sample_fmt)
        {
            return std::string(AV_NE(entry->fmt_be, entry->fmt_le));
        }
    }
    LOG(ERROR) << "sample format " << av_get_sample_fmt_name(sample_fmt) << " is not supported as output format\n";
    return std::string{};
}

static void Decode(AVCodecContext* audio_codec_context, AVPacket* packet, AVFrame* frame, FILE* fptr)
{
    std::int32_t ret{0};

    /* send the packet with the compressed data to the decoder */
    ret = avcodec_send_packet(audio_codec_context, packet);
    ASSERT_CHECK_EQ(0, ret) << "Error submitting the packet to the decoder";

    /* read all the output frames (in general there may be any number of them */
    while (ret >= 0)
    {
        ret = avcodec_receive_frame(audio_codec_context, frame);
        ASSERT_CHECK_EQ(0, ret) << "Error during decoding";

        auto data_size = av_get_bytes_per_sample(audio_codec_context->sample_fmt);
        ASSERT_CHECK_EQ(0, data_size) << "Failed to calculate data size";

        for (auto i = 0; i < frame->nb_samples; i++)
        {
            for (auto ch = 0; ch < audio_codec_context->channels; ch++)
            {
                fwrite(frame->data[ch] + data_size * i, 1, data_size, fptr);
            }
        }
    }
}
}  // namespace internal

FfmpegAudioAdapter::FfmpegAudioAdapter() { av_register_all(); }

FfmpegAudioAdapter::~FfmpegAudioAdapter() {}

/// @ref https://ffmpeg.org/doxygen/trunk/decode_audio_8c-example.html
std::pair<Waveform, std::int32_t> FfmpegAudioAdapter::Load(const std::string& path, const double offset,
                                                           const double duration, const std::int32_t& sample_rate)
{
    AVFormatContext* format_context = avformat_alloc_context();
    ASSERT_CHECK(format_context) << "Could not allocate format context";

    auto ret = avformat_open_input(&format_context, path.c_str(), nullptr, nullptr);
    ASSERT_CHECK_EQ(0, ret) << "Could not open file " << path;

    ret = avformat_find_stream_info(format_context, nullptr);
    ASSERT_CHECK_EQ(0, ret) << "Could not retrieve stream info from file " << path;

    ret = av_find_best_stream(format_context, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    ASSERT_CHECK_EQ(0, ret) << "Could not find any audio stream in the file " << path;

    auto stream_index = ret;
    AVStream* audio_stream = format_context->streams[stream_index];

    AVCodec* audio_codec = avcodec_find_decoder(audio_stream->codecpar->codec_id);
    ASSERT_CHECK(audio_codec) << "Failed to find {" << audio_stream->codecpar->codec_id << "} codec";

    AVCodecContext* audio_codec_context = avcodec_alloc_context3(audio_codec);
    ASSERT_CHECK(audio_codec_context) << "Failed to allocate {" << audio_stream->codecpar->codec_id
                                      << "} codec context.";

    ret = avcodec_parameters_to_context(audio_codec_context, audio_stream->codecpar);
    ASSERT_CHECK_EQ(0, ret) << "Failed to copy {" << audio_stream->codecpar->codec_id
                            << "} codec parameters to decoder context.";

    ret = avcodec_open2(audio_codec_context, audio_codec, nullptr);
    ASSERT_CHECK_EQ(0, ret) << "Failed to open {" << audio_stream->codecpar->codec_id << "} codec";

    AVFrame* frame = av_frame_alloc();
    ASSERT_CHECK(frame) << "Could not allocate frame";

    AVPacket packet;

    av_init_packet(&packet);
    packet.data = nullptr;
    packet.size = 0;

    while (av_read_frame(format_context, &packet) >= 0)
    {
        AVPacket temp_packet = packet;
        do
        {
            std::int32_t got_frame{0};
            ret = avcodec_decode_audio4(audio_codec_context, frame, &got_frame, &temp_packet);
            if (ret < 0)
            {
                LOG(WARN) << "Failed to decode the frame";
                break;
            }
            if (got_frame)
            {
                auto unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample(audio_codec_context->sample_fmt);

                /// @todo Copy the decoded data to buffer

                // fwrite(frame->extended_data[0], 1, unpadded_linesize, data[0]);
                av_frame_unref(frame);
            }

            temp_packet.size -= ret;
            temp_packet.data += ret;
        } while (temp_packet.size > 0);
        av_packet_unref(&packet);
    }

    av_frame_free(&frame);
    avcodec_close(audio_codec_context);
    avformat_close_input(&format_context);

    LOG(DEBUG) << "Decoded Waveform: \n"
               << " (+) sample_rate: " << audio_codec_context->sample_rate;

    return std::make_pair(Waveform{}, audio_codec_context->sample_rate);
}

/// @ref https://ffmpeg.org/doxygen/trunk/encode_audio_8c-example.html
void FfmpegAudioAdapter::Save(const std::string& path, const Waveform& data, const std::int32_t& sample_rate,
                              const std::string& codec, const std::int32_t& bitrate)
{
    LOG(INFO) << "Audio encoding with following params: \n"
              << " (+) codec: " << codec << "\n"
              << " (+) sample_rate: " << sample_rate << "\n"
              << " (+) bitrate: " << bitrate << "\n"
              << " (+) path: " << path;

    AVCodec* audio_codec = avcodec_find_encoder(AV_CODEC_ID_MP2);
    ASSERT_CHECK(audio_codec) << "Could not find codec {" << codec << "}";

    AVCodecContext* audio_codec_context = avcodec_alloc_context3(audio_codec);
    ASSERT_CHECK(audio_codec_context) << "Could not allocate context";

    audio_codec_context->bit_rate = bitrate;
    audio_codec_context->sample_rate = sample_rate;
    audio_codec_context->sample_fmt = AV_SAMPLE_FMT_S16;
    audio_codec_context->channel_layout = internal::SelectChannelLayout(audio_codec);
    audio_codec_context->channels = av_get_channel_layout_nb_channels(audio_codec_context->channel_layout);

    auto ret = avcodec_open2(audio_codec_context, audio_codec, nullptr);
    ASSERT_CHECK_EQ(0, ret) << "Could not open the context with the audio codec";

    AVPacket* packet = av_packet_alloc();
    ASSERT_CHECK(packet) << "Could not allocate packet";

    AVFrame* frame = av_frame_alloc();
    ASSERT_CHECK(frame) << "Could not allocate frame";

    frame->nb_samples = audio_codec_context->frame_size;
    frame->format = audio_codec_context->sample_fmt;
    frame->channel_layout = audio_codec_context->channel_layout;

    ret = av_frame_get_buffer(frame, 0);
    ASSERT_CHECK_EQ(0, ret) << "Could not allocate data buffers to frame";

    ret = avcodec_send_frame(audio_codec_context, frame);
    ASSERT_CHECK_EQ(0, ret) << "Could not send frame to encoder";

    /// @todo Copy raw data to frame buffer to be used for encoding.

    // std::copy_n(data[0].begin(), frame->nb_samples, frame->data[0]);
    // std::copy_n(data[1].begin(), frame->nb_samples, frame->data[1]);

    // auto fptr = fopen(path.c_str(), "wb");
    // ASSERT_CHECK(fptr) << "Could not create/open file {" << path << "}";
    std::vector<std::uint8_t> buffer;
    buffer.reserve(frame->nb_samples);
    while (ret > 0)
    {
        ret = avcodec_receive_packet(audio_codec_context, packet);
        ASSERT_CHECK_EQ(0, ret) << "Could not encode frame";

        // fwrite(packet->data, 1, packet->size, fptr);
        av_packet_unref(packet);
    }

    // fclose(fptr);
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_close(audio_codec_context);
    avcodec_free_context(&audio_codec_context);

    LOG(DEBUG) << "Encoded Waveform: \n"
               //    << " (+) size: " << data[0].size() << "x" << data[1].size() << "\n"
            //    << " (+) sample_rate: " << audio_codec_context->sample_rate << "\n"
               << " (+) path: " << path;
}
}  // namespace spleeter
