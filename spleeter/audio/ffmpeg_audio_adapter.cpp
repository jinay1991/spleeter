///
/// @file
/// @brief Contains definitions for FFMPEG Audio Adapter class methods
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/audio/ffmpeg_audio_adapter.h"

#include "spleeter/logging/logging.h"

#include <algorithm>

namespace spleeter
{
#define MAX_AUDIO_FRAME_SIZE 192000  // 1 second of 48khz 32bit audio

namespace internal
{
/// @brief Encode given frame to the media file
///
/// @param frame [in/out] - frame to be written to media file
/// @param audio_codec_context [in/out] - encoder context
/// @param format_context [in/out] - media format context
/// @param data_present [out] - writes 1 on encoded data is preset, 0 otherwise.
///
/// @return ret value - 0 on success, AVERROR (negative value) on error. Exception 0 on EOF.
static std::int32_t Encode(AVFrame* frame,
                           AVCodecContext* audio_codec_context,
                           AVFormatContext* format_context,
                           std::int32_t* data_present)
{
    AVPacket packet;
    av_init_packet(&packet);
    packet.data = nullptr;
    packet.size = 0;
    *data_present = 0;

    if (frame)
    {
        static std::int64_t pts{0};
        frame->pts = pts;
        pts += frame->nb_samples;
    }
    auto ret = avcodec_send_frame(audio_codec_context, frame);
    ASSERT_CHECK_LE(0, ret) << "Failed to send frame to encoder (Returned: " << ret << ")";

    while (ret >= 0)
    {
        ret = avcodec_receive_packet(audio_codec_context, &packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            break;
        }
        ASSERT_CHECK_LE(0, ret) << "Error during encoding (Returned: " << ret << ")";

        *data_present = 1;
        packet.stream_index = 0;
        ret = av_write_frame(format_context, &packet);
        ASSERT_CHECK_LE(0, ret) << "Failed to write frame. (Returned: " << ret << ")";
        av_packet_unref(&packet);
    }

    return 0;
}
}  // namespace internal

FfmpegAudioAdapter::FfmpegAudioAdapter()
{
    av_register_all();
}

Waveform FfmpegAudioAdapter::Load(const std::string& path,
                                  const double /*offset*/,
                                  const double /*duration*/,
                                  const std::int32_t sample_rate)
{
    ///
    /// Open Input Audio
    ///
    AVFormatContext* format_context = avformat_alloc_context();
    ASSERT_CHECK(format_context) << "Failed to allocate format context";

    auto ret = avformat_open_input(&format_context, path.c_str(), nullptr, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Failed to open file " << path;

    ret = avformat_find_stream_info(format_context, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Failed to retrieve stream info from file " << path;

    ret = av_find_best_stream(format_context, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    ASSERT_CHECK_LE(0, ret) << "Failed find any audio stream in the file " << path;
    auto stream_index = ret;
    AVStream* audio_stream = format_context->streams[stream_index];

    AVCodec* audio_codec = avcodec_find_decoder(audio_stream->codecpar->codec_id);
    ASSERT_CHECK(audio_codec) << "Failed to find {" << audio_stream->codecpar->codec_id << "} codec";

    AVCodecContext* audio_codec_context = avcodec_alloc_context3(audio_codec);
    ASSERT_CHECK(audio_codec_context) << "Failed to allocate {" << audio_stream->codecpar->codec_id
                                      << "} codec context.";

    ret = avcodec_parameters_to_context(audio_codec_context, audio_stream->codecpar);
    ASSERT_CHECK_LE(0, ret) << "Failed to copy {" << audio_stream->codecpar->codec_id
                            << "} codec parameters to decoder context.";

    ret = avcodec_open2(audio_codec_context, audio_codec, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Failed to open {" << audio_stream->codecpar->codec_id << "} codec";

    av_dump_format(format_context, 0, path.c_str(), 0);

    ///
    /// Read Audio
    ///

    SwrContext* swr_context = swr_alloc();
    ASSERT_CHECK(swr_context) << "Failed to allocate resampler.";

    swr_context = swr_alloc_set_opts(swr_context,
                                     AV_CH_LAYOUT_STEREO,
                                     AV_SAMPLE_FMT_FLT,
                                     sample_rate,
                                     av_get_default_channel_layout(audio_codec_context->channels),
                                     audio_codec_context->sample_fmt,
                                     audio_codec_context->sample_rate,
                                     0,
                                     nullptr);
    ASSERT_CHECK(swr_context) << "Failed to set options for resampler.";

    ret = swr_init(swr_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to initialize resampler. (Returned: " << ret << ")";

    std::uint8_t* buffer = (std::uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
    ASSERT_CHECK(buffer) << "Failed to allocate buffer";

    AVPacket packet;
    av_init_packet(&packet);

    Waveform data{};
    std::int32_t nb_samples{0};
    while (av_read_frame(format_context, &packet) >= 0)
    {
        AVFrame* frame = av_frame_alloc();
        ASSERT_CHECK(frame) << "Failed to allocate frame";

        ret = avcodec_send_packet(audio_codec_context, &packet);
        ASSERT_CHECK_LE(0, ret) << "Failed to send packet for decoding. (Returned: " << ret << ")";
        while (ret >= 0)
        {
            ret = avcodec_receive_frame(audio_codec_context, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            {
                break;
            }
            ASSERT_CHECK_EQ(0, ret) << "Failed to decode received packet. (Returned: " << ret << ")";

            auto buffer_size = frame->nb_samples * av_get_bytes_per_sample(audio_codec_context->sample_fmt);
            ret = swr_convert(swr_context, &buffer, buffer_size, (const std::uint8_t**)frame->data, frame->nb_samples);
            ASSERT_CHECK_LE(0, ret) << "Failed to resample. (Returned: " << ret << ")";

            for (auto idx = 0; idx < buffer_size; ++idx)
            {
                data.push_back(buffer[idx]);
            }

            nb_samples += frame->nb_samples;
        }

        av_frame_free(&frame);
        av_packet_unref(&packet);
    }
    /// Update Audio properties before releasing resources
    audio_properties_.nb_channels = audio_codec_context->channels;
    audio_properties_.nb_frames = nb_samples;
    audio_properties_.sample_rate = sample_rate;

    av_packet_unref(&packet);
    av_free(buffer);
    swr_free(&swr_context);
    avcodec_close(audio_codec_context);
    avformat_close_input(&format_context);

    SPLEETER_LOG(DEBUG) << "Decodec waveform with " << audio_properties_;
    SPLEETER_LOG(DEBUG) << "Loaded waveform from " << path << " using FFMPEG.";
    return data;
}

void FfmpegAudioAdapter::Save(const std::string& path,
                              const Waveform& data,
                              const std::int32_t sample_rate,
                              const std::string& /*codec*/,
                              const std::int32_t bitrate)
{
    ///
    /// Open Output Audio
    ///
    AVFormatContext* format_context{nullptr};
    auto ret = avformat_alloc_output_context2(&format_context, nullptr, nullptr, path.c_str());
    ASSERT_CHECK_LE(0, ret) << "Failed to deduce output format from the file extension. (Returned: " << ret << ")";

    AVOutputFormat* output_format = format_context->oformat;
    ASSERT_CHECK(output_format) << "Failed to find output format";

    AVCodec* audio_codec = avcodec_find_encoder(output_format->audio_codec);
    ASSERT_CHECK(audio_codec) << "Failed to find encoder for '" << avcodec_get_name(output_format->audio_codec)
                              << "' codec.";

    AVStream* audio_stream = avformat_new_stream(format_context, nullptr);
    ASSERT_CHECK(audio_stream) << "Failed to allocate stream";

    AVCodecContext* audio_codec_context = avcodec_alloc_context3(audio_codec);
    ASSERT_CHECK(audio_codec_context) << "Failed to allocate encoding context";

    ///
    /// Adjust Encoding Parameters
    ///
    audio_codec_context->codec_id = format_context->audio_codec_id;
    audio_codec_context->codec_type = AVMEDIA_TYPE_AUDIO;
    audio_codec_context->sample_fmt = audio_codec->sample_fmts ? audio_codec->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
    audio_codec_context->sample_rate =
        audio_codec->supported_samplerates ? audio_codec->supported_samplerates[0] : sample_rate;
    audio_codec_context->channel_layout = AV_CH_LAYOUT_STEREO;
    audio_codec_context->channels = av_get_channel_layout_nb_channels(audio_codec_context->channel_layout);
    audio_codec_context->bit_rate = bitrate;

    audio_stream->time_base = AVRational{1, sample_rate};

    if (output_format->flags & AVFMT_GLOBALHEADER)
    {
        audio_codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    ///
    /// Open Codec
    ///
    ret = avcodec_open2(audio_codec_context, audio_codec, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Failed to open the context with the audio codec {"
                            << avcodec_get_name(output_format->audio_codec) << "}. (Returned: " << ret << ")";

    ret = avcodec_parameters_from_context(audio_stream->codecpar, audio_codec_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to copy {" << audio_stream->codecpar->codec_id
                            << "} codec parameters to decoder context. (Returned: " << ret << ")";
    av_dump_format(format_context, 0, path.c_str(), 1);

    if (!(format_context->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&format_context->pb, path.c_str(), AVIO_FLAG_WRITE);
        ASSERT_CHECK_LE(0, ret) << "Failed to open " << path << ". (Returned: " << ret << ")";
        SPLEETER_LOG(DEBUG) << "Successfully opened " << path << " for writing.";
    }

    if (audio_codec_context->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
    {
        audio_codec_context->frame_size =
            av_rescale_rnd(data.size() / av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO),
                           sample_rate,
                           sample_rate,
                           AV_ROUND_UP);
    }

    ///
    /// Write Audio
    ///
    AVFrame* frame = av_frame_alloc();
    ASSERT_CHECK(frame) << "Failed to allocate frame";

    frame->nb_samples = audio_codec_context->frame_size;
    frame->format = audio_codec_context->sample_fmt;
    frame->channels = audio_codec_context->channels;
    frame->channel_layout = audio_codec_context->channel_layout;
    frame->sample_rate = audio_codec_context->sample_rate;

    ret = av_frame_get_buffer(frame, 0);
    ASSERT_CHECK_LE(0, ret) << "Failed to allocate audio data buffers. (Returned: " << ret << ")";

    ret = av_frame_make_writable(frame);
    ASSERT_CHECK_LE(0, ret) << "Failed to make frame writable. (Returned: " << ret << ")";

    ///
    /// Allocate Resampler
    ///
    SwrContext* swr_context = swr_alloc();
    ASSERT_CHECK(swr_context) << "Failed to allocate resampler.";

    swr_context = swr_alloc_set_opts(swr_context,
                                     audio_codec_context->channel_layout,
                                     audio_codec_context->sample_fmt,
                                     audio_codec_context->sample_rate,
                                     AV_CH_LAYOUT_STEREO,
                                     AV_SAMPLE_FMT_FLTP,
                                     sample_rate,
                                     0,
                                     nullptr);
    ASSERT_CHECK(swr_context) << "Failed to set options for resampler.";

    ret = swr_init(swr_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to initialize resampler. (Returned: " << ret << ")";

    auto buffer_size = av_samples_get_buffer_size(
        nullptr, audio_codec_context->channels, audio_codec_context->frame_size, audio_codec_context->sample_fmt, 0);
    ASSERT_CHECK_LE(0, buffer_size) << "Failed to calculate output buffer size. (Returned: " << buffer_size << ")";

    auto buffer = (std::uint8_t*)av_malloc(buffer_size);
    ASSERT_CHECK(buffer) << "Failed to allocate array. (Returned: " << ret << ")";

    std::uint8_t** src_data{nullptr};
    std::int32_t src_linesize{0};
    std::uint8_t** dst_data{nullptr};
    std::int32_t dst_linesize{0};
    ret = av_samples_alloc_array_and_samples(&src_data,
                                             &src_linesize,
                                             av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO),
                                             (data.size() / av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO)),
                                             AV_SAMPLE_FMT_FLTP,
                                             0);
    ASSERT_CHECK_LE(0, ret) << "Failed to allocate src samples. (Returned: " << ret << ")";

    ret = av_samples_fill_arrays(src_data,
                                 &src_linesize,
                                 (const std::uint8_t*)data.data(),
                                 av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO),
                                 (data.size() / av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO)),
                                 AV_SAMPLE_FMT_FLTP,
                                 0);
    ASSERT_CHECK_LE(0, ret) << "Failed to fill src samples. (Returned: " << ret << ")";

    ret = av_samples_alloc_array_and_samples(&dst_data,
                                             &dst_linesize,
                                             audio_codec_context->channels,
                                             audio_codec_context->frame_size,
                                             audio_codec_context->sample_fmt,
                                             0);
    ASSERT_CHECK_LE(0, ret) << "Failed to allocate dst samples. (Returned: " << ret << ")";

    ret = swr_convert(swr_context,
                      dst_data,
                      audio_codec_context->frame_size,
                      (const std::uint8_t**)src_data,
                      (data.size() / av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO)));
    ASSERT_CHECK_LE(0, ret) << "Failed to convert buffer using resampler. (Returned: " << ret << ")";

    // ret = avcodec_fill_audio_frame(frame,
    //                                audio_codec_context->channels,
    //                                audio_codec_context->sample_fmt,
    //                                *dst_data,
    //                                audio_codec_context->frame_size,
    //                                1);
    // ASSERT_CHECK_LE(0, ret) << "Failed to fill audio frame. (Returned: " << ret << ")";

    ret = avformat_write_header(format_context, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Failed to write header information for " << path << ". (Returned: " << ret << ")";

    SPLEETER_LOG(INFO) << "Encoding given data {size: " << buffer_size << ", nb_samples: " << frame->nb_samples << "}";
    std::int32_t data_present{0};
    frame->data[0] = dst_data[0];
    frame->data[1] = dst_data[1];
    ret = internal::Encode(frame, audio_codec_context, format_context, &data_present);
    ASSERT_CHECK_LE(0, ret) << "Failed to encode frame. (Returned: " << ret << ")";

    ///
    /// Write queued frames
    ///
    data_present = 0;
    do
    {
        ret = internal::Encode(nullptr, audio_codec_context, format_context, &data_present);
        ASSERT_CHECK_LE(0, ret) << "Failed to encode frame. (Returned: " << ret << ")";
    } while (data_present);

    ret = av_write_trailer(format_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to write output file trailer. (Returned: " << ret << ")";

    ///
    /// Cleanup
    ///
    if (!(format_context->flags & AVFMT_NOFILE))
    {
        ret = avio_close(format_context->pb);
        ASSERT_CHECK_LE(0, ret) << "Failed to close " << path << ". (Returned: " << ret << ")";
        SPLEETER_LOG(DEBUG) << "Successfully closed " << path << " after writing.";
    }
    swr_free(&swr_context);
    av_free(buffer);
    av_frame_free(&frame);
    avformat_free_context(format_context);
    avcodec_close(audio_codec_context);
    avcodec_free_context(&audio_codec_context);

    SPLEETER_LOG(DEBUG) << "Saved waveform to " << path << " using FFMPEG.";
}

AudioProperties FfmpegAudioAdapter::GetProperties() const
{
    return audio_properties_;
}

}  // namespace spleeter
