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
FfmpegAudioAdapter::FfmpegAudioAdapter() { av_register_all(); }

/// @ref https://ffmpeg.org/doxygen/trunk/decode_audio_8c-example.html
std::pair<Waveform, std::int32_t> FfmpegAudioAdapter::Load(const std::string& path, const double offset,
                                                           const double duration, const std::int32_t& sample_rate)
{
    ///
    /// Open Input Audio
    ///
    AVFormatContext* format_context = avformat_alloc_context();
    ASSERT_CHECK(format_context) << "Could not allocate format context";

    auto ret = avformat_open_input(&format_context, path.c_str(), nullptr, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Could not open file " << path;

    ret = avformat_find_stream_info(format_context, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Could not retrieve stream info from file " << path;

    ret = av_find_best_stream(format_context, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    ASSERT_CHECK_LE(0, ret) << "Could not find any audio stream in the file " << path;
    auto stream_index = ret;
    AVStream* audio_stream = format_context->streams[stream_index];
    LOG(DEBUG) << "Given audio file {" << path << "} has {"
               << av_get_media_type_string(audio_stream->codecpar->codec_type) << "} media stream.";

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

    FILE* fptr = fopen("/tmp/decoded_audio.wav", "wb");
    ASSERT_CHECK(fptr) << "Failed to open/create output file";

    while (av_read_frame(format_context, &packet) >= 0)
    {
        AVFrame* frame = av_frame_alloc();
        ASSERT_CHECK(frame) << "Could not allocate frame";

        av_packet_rescale_ts(&packet, audio_stream->time_base, audio_codec_context->time_base);

        if (packet.stream_index == stream_index)
        {
            ret = avcodec_send_packet(audio_codec_context, &packet);
            ASSERT_CHECK_LE(0, ret) << "Error sending packet for decoding";
            while (ret >= 0)
            {
                ret = avcodec_receive_frame(audio_codec_context, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                {
                    break;
                }
                ASSERT_CHECK_LE(0, ret) << "Error during decoding";
                auto unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample(audio_codec_context->sample_fmt);

                /// @todo Copy the buffer instead of saving to file
                fwrite(frame->data[0], 1, unpadded_linesize, fptr);
            }
        }
        av_frame_free(&frame);
        av_packet_unref(&packet);
    }
    fclose(fptr);
    avcodec_close(audio_codec_context);
    avformat_close_input(&format_context);

    LOG(DEBUG) << "Decoded Waveform: \n"
               << " (+) bitrate: " << audio_codec_context->bit_rate << "\n"
               << " (+) codec: " << avcodec_get_name(audio_codec_context->codec_id) << "\n"
               << " (+) sample_rate: " << audio_codec_context->sample_rate;

    return std::make_pair(Waveform{}, audio_codec_context->sample_rate);
}

/// @ref https://ffmpeg.org/doxygen/trunk/encode_audio_8c-example.html
void FfmpegAudioAdapter::Save(const std::string& path, const Waveform& data, const std::int32_t& sample_rate,
                              const std::string& codec, const std::int32_t& bitrate)
{
    ///
    /// Open Output Audio
    ///
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

    ///
    /// Open Codec
    ///
    ret = avcodec_open2(audio_codec_context, audio_codec, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Could not open the context with the audio codec, (Returned: " << ret << ")";

    AVFrame* frame = av_frame_alloc();
    ASSERT_CHECK(frame) << "Could not allocate frame";

    frame->format = audio_codec_context->sample_fmt;
    frame->channel_layout = audio_codec_context->channel_layout;
    frame->sample_rate = audio_codec_context->sample_rate;
    frame->nb_samples = audio_codec_context->frame_size;

    ret = av_frame_get_buffer(frame, 0);
    ASSERT_CHECK_LE(0, ret) << "Could not allocate data buffers to frame, (Returned: " << ret << ")";

    ret = avcodec_parameters_from_context(audio_stream->codecpar, audio_codec_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to copy {" << audio_stream->codecpar->codec_id
                            << "} codec parameters to decoder context, (Returned: " << ret << ")";

    auto swr_context = swr_alloc();
    ASSERT_CHECK(swr_context) << "Could not allocate resampler context";

    av_opt_set_int(swr_context, "in_channel_count", audio_codec_context->channels, 0);
    av_opt_set_int(swr_context, "in_sample_rate", audio_codec_context->sample_rate, 0);
    av_opt_set_sample_fmt(swr_context, "in_sample_fmt", AV_SAMPLE_FMT_S16, 0);
    av_opt_set_int(swr_context, "out_channel_count", audio_codec_context->channels, 0);
    av_opt_set_int(swr_context, "out_sample_rate", audio_codec_context->sample_rate, 0);
    av_opt_set_sample_fmt(swr_context, "out_sample_fmt", audio_codec_context->sample_fmt, 0);

    ret = swr_init(swr_context);
    ASSERT_CHECK_LE(0, ret) << "Failed to initialize the resampling context";

    /// Open Output file, if needed
    av_dump_format(format_context, 0, path.c_str(), 1);

    if (!(format_context->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&format_context->pb, path.c_str(), AVIO_FLAG_WRITE);
        ASSERT_CHECK_LE(0, ret) << "Could not open " << path << ", (Returned: " << ret << ")";
    }

    /// Write the stream header, if any
    ret = avformat_write_header(format_context, nullptr);
    ASSERT_CHECK_LE(0, ret) << "Error occurred when opening output file, (Returned: " << ret << ")";

    std::uint32_t sample_count{0};
    std::int32_t got_packet{1};
    while (got_packet)
    {
        AVPacket packet{};

        av_init_packet(&packet);

        /// Write buffer to frame
        {
            std::uint16_t* buffer = (std::uint16_t*)frame->data[0];
            for (auto j = 0; j < frame->nb_samples; ++j)
            {
                auto value = data[j];
                for (auto i = 0; i < audio_codec_context->channels; ++i)
                {
                    *buffer++ = value;
                }
            }

            // convert samples from native format to destination codec format, using the resampler compute destination
            // number of samples
            auto nb_samples =
                av_rescale_rnd(swr_get_delay(swr_context, audio_codec_context->sample_rate) + frame->nb_samples,
                               audio_codec_context->sample_rate, audio_codec_context->sample_rate, AV_ROUND_UP);
            ASSERT_CHECK_EQ(nb_samples, frame->nb_samples);

            ret = av_frame_make_writable(frame);
            ASSERT_CHECK_LE(0, ret) << "Unable to make writable";

            ret = swr_convert(swr_context, frame->data, nb_samples, (const std::uint8_t**)(frame->data),
                              frame->nb_samples);
            ASSERT_CHECK_LE(0, ret) << "Error while converting";

            frame->pts = av_rescale_q(sample_count, AVRational{1, audio_codec_context->sample_rate},
                                      audio_codec_context->time_base);
            sample_count += nb_samples;
        }

        ret = avcodec_encode_audio2(audio_codec_context, &packet, frame, &got_packet);
        ASSERT_CHECK_LE(0, ret) << "Error encoding audio frame";

        if (got_packet)
        {
            av_packet_rescale_ts(&packet, audio_codec_context->time_base, audio_stream->time_base);
            packet.stream_index = audio_stream->index;

            ret = av_interleaved_write_frame(format_context, &packet);
            ASSERT_CHECK_LE(0, ret) << "Error while writing audio";
        }
    }

    av_write_trailer(format_context);

    if (!(format_context->flags & AVFMT_NOFILE))
    {
        avio_closep(&format_context->pb);
    }
    av_frame_free(&frame);
    avformat_free_context(format_context);
    avcodec_close(audio_codec_context);
    avcodec_free_context(&audio_codec_context);

    LOG(DEBUG) << "Encoded Waveform: \n"
               //    << " (+) size: " << data[0].size() << "x" << data[1].size() << "\n"
               //    << " (+) sample_rate: " << audio_codec_context->sample_rate << "\n"
               << " (+) path: " << path;
}
}  // namespace spleeter
