///
/// @file
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
    LOG(INFO) << "Audio frame info:\n"
              << "  Sample count: " << frame->nb_samples << '\n'
              << "  Channel count: " << codec_ctx->channels << '\n'
              << "  Format: " << av_get_sample_fmt_name(codec_ctx->sample_fmt) << '\n'
              << "  Bytes per sample: " << av_get_bytes_per_sample(codec_ctx->sample_fmt) << '\n'
              << "  Is planar? " << av_sample_fmt_is_planar(codec_ctx->sample_fmt) << '\n';

    LOG(INFO) << "frame->linesize[0] tells you the size (in bytes) of each plane\n";

    if (codec_ctx->channels > AV_NUM_DATA_POINTERS && av_sample_fmt_is_planar(codec_ctx->sample_fmt))
    {
        LOG(INFO) << "The audio stream (and its frames) have too many channels to fit in\n"
                  << "frame->data. Therefore, to access the audio data, you need to use\n"
                  << "frame->extended_data to access the audio data. It's planar, so\n"
                  << "each channel is in a different element. That is:\n"
                  << "  frame->extended_data[0] has the data for channel 1\n"
                  << "  frame->extended_data[1] has the data for channel 2\n"
                  << "  etc.\n";
    }
    else
    {
        LOG(INFO) << "Either the audio data is not planar, or there is enough room in\n"
                  << "frame->data to store all the channels, so you can either use\n"
                  << "frame->data or frame->extended_data to access the audio data (they\n"
                  << "should just point to the same data).\n";
    }

    LOG(INFO) << "If the frame is planar, each channel is in a different element.\n"
              << "That is:\n"
              << "  frame->data[0]/frame->extended_data[0] has the data for channel 1\n"
              << "  frame->data[1]/frame->extended_data[1] has the data for channel 2\n"
              << "  etc.\n";

    LOG(INFO) << "If the frame is packed (not planar), then all the data is in\n"
              << "frame->data[0]/frame->extended_data[0] (kind of like how some\n"
              << "image formats have RGB pixels packed together, rather than storing\n"
              << " the red, green, and blue channels separately in different arrays.\n";
}
}  // namespace internal
FfmpegAudioAdapter::FfmpegAudioAdapter() : format_{nullptr}
{
    av_register_all();
    format_ = avformat_alloc_context();
}

FfmpegAudioAdapter::~FfmpegAudioAdapter()
{
    av_free(frame_);
    avcodec_close(codec_ctx_);
    avformat_close_input(&format_);
}

std::tuple<Waveform, std::int32_t> FfmpegAudioAdapter::Load(const std::string& path, const double offset,
                                                            const double duration, const std::int32_t& sample_rate)
{
    std::int32_t ret = 0;

    frame_ = av_frame_alloc();
    ASSERT_CHECK(frame_) << "Error allocating the frame";

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
    codec_ctx_ = audio_stream->codec;
    codec_ctx_->codec = codec;

    ret = avcodec_open2(codec_ctx_, codec_ctx_->codec, NULL);
    ASSERT_CHECK(ret == 0) << "Couldn't open the context with the decoder";

    LOG(INFO) << "Stream has " << codec_ctx_->channels << " channels and a sample rate of " << codec_ctx_->sample_rate
              << "Hz";
    LOG(INFO) << "Data is in the format " << av_get_sample_fmt_name(codec_ctx_->sample_fmt);

    AVPacket packet;
    av_init_packet(&packet);
    // Read the packets in a loop
    while (av_read_frame(format_, &packet) == 0)
    {
        if (packet.stream_index == audio_stream->index)
        {
            AVPacket decoded_packet = packet;

            // Audio packets can have multiple audio frames in a single packet
            while (decoded_packet.size > 0)
            {
                // Try to decode the packet into a frame
                // Some frames rely on multiple packets, so we have to make sure the frame is finished before
                // we can use it
                int gotFrame = 0;
                int result = avcodec_decode_audio4(codec_ctx_, frame_, &gotFrame, &decoded_packet);

                if (result >= 0 && gotFrame)
                {
                    decoded_packet.size -= result;
                    decoded_packet.data += result;

                    // We now have a fully decoded audio frame
                    internal::PrintAudioFrameInfo(codec_ctx_, frame_);
                }
                else
                {
                    decoded_packet.size = 0;
                    decoded_packet.data = nullptr;
                }
            }
        }
        // You *must* call av_free_packet() after each call to av_read_frame() or else you'll leak memory
        av_free_packet(&packet);
    }
    // Some codecs will cause frames to be buffered up in the decoding process. If the CODEC_CAP_DELAY flag
    // is set, there can be buffered up frames that need to be flushed, so we'll do that
    if (codec_ctx_->codec->capabilities & CODEC_CAP_DELAY)
    {
        av_init_packet(&packet);
        // Decode all the remaining frames in the buffer, until the end is reached
        int frame_received = 0;
        while (avcodec_decode_audio4(codec_ctx_, frame_, &frame_received, &packet) >= 0 && frame_received)
        {
            // We now have a fully decoded audio frame
            internal::PrintAudioFrameInfo(codec_ctx_, frame_);
        }
    }

    return std::make_tuple<Waveform, std::int32_t>("", 0);
}

/// @brief Write waveform data to the file denoted by the given path using FFMPEG process.
void FfmpegAudioAdapter::Save(const std::string& path, const std::string& data, const std::int32_t& sample_rate,
                              const std::string& codec)
{
}
}  // namespace spleeter
