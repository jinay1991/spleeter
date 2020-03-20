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

static void Encode(AVCodecContext* audio_codec_context, AVPacket* packet, AVFrame* frame, FILE* fptr)
{
    std::int32_t ret{0};

    /* send the packet with the compressed data to the decoder */
    ret = avcodec_send_packet(audio_codec_context, packet);
    ASSERT_CHECK_EQ(0, ret) << "Error submitting the packet to the decoder " << ret << " AVERROR_EOF: " << AVERROR_EOF
                            << " AVERROR(EINVAL): " << EINVAL << " AVERROR(ENOMEM): " << ENOMEM
                            << " AVERROR(EAGAIN): " << EAGAIN;

    /* read all the output frames (in general there may be any number of them */
    while (ret >= 0)
    {
        ret = avcodec_receive_frame(audio_codec_context, frame);
        ASSERT_CHECK_LE(0, ret) << "Error decoding audio frame";

        fwrite(packet->data, 1, packet->size, fptr);
        av_packet_unref(packet);
    }
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
        ASSERT_CHECK_LE(0, ret) << "Error decoding audio frame";

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

    FILE* fptr = fopen("/tmp/decoded_audio.raw", "wb");
    ASSERT_CHECK(fptr) << "Failed to open/create output file";

    while (av_read_frame(format_context, &packet) >= 0)
    {
        if (packet.stream_index == stream_index)
        {
            ret = avcodec_send_packet(audio_codec_context, &packet);
            ASSERT_CHECK_LE(0, ret) << "Error sending packet for decoding";
            while (ret >= 0)
            {
                std::int32_t got_frame{0};
                ret = avcodec_receive_frame(audio_codec_context, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                {
                    break;
                }
                ASSERT_CHECK_LE(0, ret) << "Error during decoding";
                auto unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample(audio_codec_context->sample_fmt);

                fwrite(frame->data[0], 1, unpadded_linesize, fptr);
            }
        }
        av_packet_unref(&packet);
    }
    fclose(fptr);

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
    LOG(INFO) << "Received " << (data.size() / 1000) << " Kbytes for encoding...";
    AVFormatContext* format_context{nullptr};
    auto ret = avformat_alloc_output_context2(&format_context, nullptr, nullptr, path.c_str());
    ASSERT_CHECK_LE(0, ret) << "Cound not deduce output format from the file extension, (Returned: " << ret << ")";

    AVOutputFormat* output_format = format_context->oformat;

    AVCodec* audio_codec = avcodec_find_encoder(output_format->audio_codec);
    ASSERT_CHECK(audio_codec) << "Could not find encoder for " << output_format->audio_codec << "codec.";

    AVStream* audio_stream = avformat_new_stream(format_context, nullptr);
    ASSERT_CHECK(audio_stream) << "Could not allocate stream";

    audio_stream->id = format_context->nb_streams - 1;

    AVCodecContext* audio_codec_context = avcodec_alloc_context3(audio_codec);
    ASSERT_CHECK(audio_codec_context) << "Could not allocate encoding context";

    audio_codec_context->sample_fmt = audio_codec->sample_fmts ? audio_codec->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
    audio_codec_context->bit_rate = bitrate;
    audio_codec_context->sample_rate = sample_rate;
    if (audio_codec->supported_samplerates)
    {
        audio_codec_context->sample_rate = audio_codec->supported_samplerates[0];
        for (auto i = 0; audio_codec->supported_samplerates[i]; i++)
        {
            if (audio_codec->supported_samplerates[i] == sample_rate)
            {
                audio_codec_context->sample_rate = sample_rate;
            }
        }
    }
    audio_codec_context->channels = av_get_channel_layout_nb_channels(audio_codec_context->channel_layout);
    audio_codec_context->channel_layout = AV_CH_LAYOUT_STEREO;
    if (audio_codec->channel_layouts)
    {
        audio_codec_context->channel_layout = audio_codec->channel_layouts[0];
        for (auto i = 0; audio_codec->channel_layouts[i]; i++)
        {
            if (audio_codec->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
            {
                audio_codec_context->channel_layout = AV_CH_LAYOUT_STEREO;
            }
        }
    }
    audio_codec_context->channels = av_get_channel_layout_nb_channels(audio_codec_context->channel_layout);
    audio_stream->time_base = (AVRational){1, audio_codec_context->sample_rate};

    if (format_context->oformat->flags & AVFMT_GLOBALHEADER)
    {
        audio_codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    ret = avcodec_open2(audio_codec_context, audio_codec, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Could not open the context with the audio codec, (Returned: " << ret << ")";

    AVPacket* packet = av_packet_alloc();
    ASSERT_CHECK(packet) << "Could not allocate packet";

    AVFrame* frame = av_frame_alloc();
    ASSERT_CHECK(frame) << "Could not allocate frame";

    frame->nb_samples = audio_codec_context->frame_size;
    frame->format = audio_codec_context->sample_fmt;
    frame->channel_layout = audio_codec_context->channel_layout;

    ret = av_frame_get_buffer(frame, 0);
    ASSERT_CHECK_LE(0, ret) << "Could not allocate data buffers to frame, (Returned: " << ret << ")";

    ret = avcodec_parameters_from_context(audio_stream->codecpar, audio_codec_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to copy {" << audio_stream->codecpar->codec_id
                            << "} codec parameters to decoder context, (Returned: " << ret << ")";

    av_dump_format(format_context, 0, path.c_str(), 1);

    if (!(format_context->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&format_context->pb, path.c_str(), AVIO_FLAG_WRITE);
        ASSERT_CHECK_LE(0, ret) << "Could not open " << path << ", (Returned: " << ret << ")";
    }
    ret = avformat_init_output(format_context, nullptr);

    ret = avformat_write_header(format_context, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Error occurred when opening output file, (Returned: " << ret << ")";

    av_init_packet(packet);

    std::uint8_t* buffer = frame->data[0];
    for (auto j = 0; j < frame->nb_samples; ++j)
    {
        auto value = data[j];
        for (auto i = 0; i < audio_codec_context->channels; ++i)
        {
            *buffer++ = value;
        }
    }

    ret = av_frame_make_writable(frame);
    ASSERT_CHECK_EQ(0, ret) << "Unable to make writable";

    std::int32_t got_frame{0};
    ret = avcodec_encode_audio2(audio_codec_context, packet, frame, &got_frame);
    ASSERT_CHECK_LE(0, ret) << "Error encoding audio frame";

    if (got_frame)
    {
        av_packet_rescale_ts(packet, audio_codec_context->time_base, audio_stream->time_base);
        packet->stream_index = audio_stream->index;

        ret = av_interleaved_write_frame(format_context, packet);
        ASSERT_CHECK_LE(0, ret) << "Error while writing audio";
    }
    av_write_trailer(format_context);

    if (!(format_context->flags & AVFMT_NOFILE))
    {
        avio_closep(&format_context->pb);
    }
    av_frame_free(&frame);
    av_packet_free(&packet);
    avformat_free_context(format_context);
    avcodec_close(audio_codec_context);
    avcodec_free_context(&audio_codec_context);

    LOG(DEBUG) << "Encoded Waveform: \n"
               //    << " (+) size: " << data[0].size() << "x" << data[1].size() << "\n"
               //    << " (+) sample_rate: " << audio_codec_context->sample_rate << "\n"
               << " (+) path: " << path;
}
}  // namespace spleeter
