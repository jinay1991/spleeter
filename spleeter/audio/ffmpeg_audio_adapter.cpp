///
/// @file
/// @brief Contains definitions for FFMPEG Audio Adapter class methods
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/audio/ffmpeg_audio_adapter.h"
#include "spleeter/logging/logging.h"

namespace spleeter
{
#define MAX_AUDIO_FRAME_SIZE 192000  // 1 second of 48khz 32bit audio

FfmpegAudioAdapter::FfmpegAudioAdapter() { av_register_all(); }

Waveform FfmpegAudioAdapter::Load(const std::string& path, const double /*offset*/, const double /*duration*/,
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
    AVPacket packet;
    av_init_packet(&packet);

    SwrContext* swr_context = swr_alloc();
    ASSERT_CHECK(swr_context) << "Failed to allocate resampler.";

    swr_context = swr_alloc_set_opts(swr_context, AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_FLT, sample_rate,
                                     av_get_default_channel_layout(audio_codec_context->channels),
                                     audio_codec_context->sample_fmt, audio_codec_context->sample_rate, 0, nullptr);
    ASSERT_CHECK(swr_context) << "Failed to set options for resampler.";

    ret = swr_init(swr_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to initialize resampler. (Returned: " << ret << ")";

    std::uint8_t* buffer = (std::uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
    ASSERT_CHECK(buffer) << "Failed to allocate buffer";

    Waveform data{};
    std::int32_t nb_samples{0};
    while (av_read_frame(format_context, &packet) >= 0)
    {
        AVFrame* frame = av_frame_alloc();
        ASSERT_CHECK(frame) << "Failed to allocate frame";

        av_packet_rescale_ts(&packet, audio_stream->time_base, audio_codec_context->time_base);

        if (packet.stream_index == stream_index)
        {
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

                auto unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample(audio_codec_context->sample_fmt);
                ret = swr_convert(swr_context, &buffer, unpadded_linesize, (const std::uint8_t**)frame->data,
                                  frame->nb_samples);
                ASSERT_CHECK_LE(0, ret) << "Failed to resample. (Returned: " << ret << ")";

                /// copy to file as well.
                for (auto idx = 0; idx < unpadded_linesize; ++idx)
                {
                    data.push_back(buffer[idx]);
                }

                /// frame
                nb_samples += frame->nb_samples;
            }
        }

        av_frame_free(&frame);
        av_packet_unref(&packet);
    }
    /// Update Audio properties before releasing resources
    audio_properties_.nb_channels = audio_codec_context->channels;
    audio_properties_.nb_frames = nb_samples;
    audio_properties_.sample_rate = sample_rate;

    av_free(buffer);
    swr_free(&swr_context);
    avcodec_close(audio_codec_context);
    avformat_close_input(&format_context);

    SPLEETER_LOG(DEBUG) << "Loaded waveform from " << path << " using FFMPEG.";
    return data;
}

void FfmpegAudioAdapter::Save(const std::string& path, const Waveform& data, const std::int32_t sample_rate,
                              const std::string& codec, const std::int32_t bitrate)
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
    ASSERT_CHECK(audio_codec) << "Failed to find encoder for " << output_format->audio_codec << "codec.";

    AVStream* audio_stream = avformat_new_stream(format_context, nullptr);
    ASSERT_CHECK(audio_stream) << "Failed to allocate stream";

    AVCodecContext* audio_codec_context = avcodec_alloc_context3(audio_codec);
    ASSERT_CHECK(audio_codec_context) << "Failed to allocate encoding context";

    ///
    /// Adjust Encoding Parameters
    ///
    audio_codec_context->codec_id = format_context->audio_codec_id;
    audio_codec_context->bit_rate = bitrate;
    audio_codec_context->sample_rate = sample_rate;
    audio_codec_context->sample_fmt = AV_SAMPLE_FMT_S16;
    // audio_codec_context->time_base = AVRational{1, audio_codec_context->sample_rate};
    audio_codec_context->channel_layout = AV_CH_LAYOUT_STEREO;
    audio_codec_context->codec_type = AVMEDIA_TYPE_AUDIO;
    audio_codec_context->channels = av_get_channel_layout_nb_channels(audio_codec_context->channel_layout);
    if (output_format->flags & AVFMT_GLOBALHEADER)
    {
        audio_codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    ///
    /// Open Codec
    ///
    ret = avcodec_open2(audio_codec_context, audio_codec, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Failed to open the context with the audio codec. (Returned: " << ret << ")";

    ret = avcodec_parameters_from_context(audio_stream->codecpar, audio_codec_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to copy {" << audio_stream->codecpar->codec_id
                            << "} codec parameters to decoder context. (Returned: " << ret << ")";

    av_dump_format(format_context, 0, path.c_str(), 1);

    if (!(format_context->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&format_context->pb, path.c_str(), AVIO_FLAG_WRITE);
        ASSERT_CHECK_LE(0, ret) << "Failed to open " << path << ". (Returned: " << ret << ")";
    }
    // FILE* fout = fopen(path.c_str(), "wb");
    // ASSERT_CHECK(fout) << "Failed to open output file {" << path << "} for writing.";

    SwrContext* swr_context = swr_alloc();
    ASSERT_CHECK(swr_context) << "Failed to allocate resampler";

    swr_context = swr_alloc_set_opts(swr_context, AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, sample_rate,
                                     audio_codec_context->channel_layout, audio_codec_context->sample_fmt,
                                     audio_codec_context->sample_rate, 0, nullptr);
    ASSERT_CHECK(swr_context) << "Failed to allocate resampler";

    ret = swr_init(swr_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to initialize resampler. (Returned: " << ret << ")";

    ///
    /// Write Audio
    ///
    AVFrame* frame = av_frame_alloc();
    ASSERT_CHECK(frame) << "Failed to allocate frame";

    audio_codec_context->frame_size = data.size() / audio_codec_context->channels;

    frame->nb_samples = audio_codec_context->frame_size;
    frame->format = audio_codec_context->sample_fmt;
    frame->channel_layout = audio_codec_context->channel_layout;
    frame->sample_rate = audio_codec_context->sample_rate;

    ret = av_frame_get_buffer(frame, 1);
    ASSERT_CHECK_LE(0, ret) << "Failed to allocate data buffers to frame. (Returned: " << ret << ")";

    ret = av_frame_make_writable(frame);
    ASSERT_CHECK_LE(0, ret) << "Failed to make frame buffers writable. (Returned: " << ret << ")";

    auto frame_size = av_samples_get_buffer_size(nullptr, audio_codec_context->channels,
                                                 audio_codec_context->frame_size, audio_codec_context->sample_fmt, 1);
    ASSERT_CHECK_LT(0, frame_size) << "Failed to calculate buffer size for frame. (Returned: " << frame_size << ")";

    std::uint8_t* buffer = (std::uint8_t*)data.data();  //(std::uint8_t*)av_malloc(frame_size);
    ret = avcodec_fill_audio_frame(frame, audio_codec_context->channels, audio_codec_context->sample_fmt,
                                   (const std::uint8_t*)buffer, frame_size, 1);
    ASSERT_CHECK_LE(0, ret) << "Failed to fill audio frame. (Returned: " << ret << ")";

    ret = avformat_write_header(format_context, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Failed to write header information for " << path << ". (Returned: " << ret << ")";

    std::int32_t data_present{0};
    for (auto i = 0U; i < (data.size() / frame_size); i++, buffer += frame_size)
    {
        frame->data[0] = buffer;
        AVPacket packet;
        av_init_packet(&packet);
        packet.data = nullptr;
        packet.size = 0;

        if (frame)
        {
            static std::int64_t pts{0};
            frame->pts = pts;
            pts += frame->nb_samples;
        }
        auto ret = avcodec_send_frame(audio_codec_context, frame);
        if (ret == AVERROR_EOF)
        {
            av_packet_unref(&packet);
            break;
        }
        ASSERT_CHECK_LE(0, ret) << "Failed to send frame to encoder (Returned: " << ret << ")";

        ret = avcodec_receive_packet(audio_codec_context, &packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            av_packet_unref(&packet);
            break;
        }
        ASSERT_CHECK_LE(0, ret) << "Error during encoding (Returned: " << ret << ")";

        packet.stream_index = 0;
        /// @todo Is it saving the file properly or configuration has some issue?
        ret = av_interleaved_write_frame(format_context, &packet);
        // fwrite(packet.data, 1, packet.size, fout);
        ASSERT_CHECK_LE(0, ret) << "Failed to write frame. (Returned: " << ret << ")";
        av_packet_unref(&packet);
    }
    do
    {
        data_present = 0;
        AVPacket packet;
        av_init_packet(&packet);
        packet.data = nullptr;
        packet.size = 0;

        if (frame)
        {
            static std::int64_t pts{0};
            frame->pts = pts;
            pts += frame->nb_samples;
        }
        auto ret = avcodec_send_frame(audio_codec_context, frame);
        if (ret == AVERROR_EOF)
        {
            av_packet_unref(&packet);
            continue;
        }
        ASSERT_CHECK_LE(0, ret) << "Failed to send frame to encoder (Returned: " << ret << ")";

        ret = avcodec_receive_packet(audio_codec_context, &packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            av_packet_unref(&packet);
            continue;
        }
        ASSERT_CHECK_LE(0, ret) << "Error during encoding (Returned: " << ret << ")";

        packet.stream_index = 0;
        ret = av_interleaved_write_frame(format_context, &packet);
        // fwrite(packet.data, 1, packet.size, fout);
        ASSERT_CHECK_LE(0, ret) << "Failed to write frame. (Returned: " << ret << ")";
        av_packet_unref(&packet);
    } while (data_present);

    ret = av_write_trailer(format_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to write output file trailer. (Returned: " << ret << ")";

    if (!(format_context->flags & AVFMT_NOFILE))
    {
        avio_closep(&format_context->pb);
    }
    // fclose(fout);
    swr_free(&swr_context);
    av_frame_free(&frame);
    avformat_free_context(format_context);
    avcodec_close(audio_codec_context);
    avcodec_free_context(&audio_codec_context);

    SPLEETER_LOG(DEBUG) << "Saved waveform to " << path << " using FFMPEG.";
}

AudioProperties FfmpegAudioAdapter::GetProperties() const { return audio_properties_; }

}  // namespace spleeter
