///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_FFMPEG_AUDIO_ADAPTER_H_
#define SPLEETER_FFMPEG_AUDIO_ADAPTER_H_

#include "spleeter/i_audio_adapter.h"

#ifndef __cplusplus__
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#ifndef __cplusplus__
}
#endif
#include <string>

/**
 * References:
 * 1) https://rodic.fr/blog/libavcodec-tutorial-decode-audio-file/
 * 2) https://medium.com/@donturner/using-ffmpeg-for-faster-audio-decoding-967894e94e71  <--- Looks good.
 */

namespace spleeter
{
/// @brief FFMPEG Audio Adapter
class FfmpegAudioAdapter : public IAudioAdapter
{
  public:
    /// @brief Constructor.
    FfmpegAudioAdapter();

    /// @brief Destructor.
    virtual ~FfmpegAudioAdapter() = default;

    /// @brief Loads the audio file denoted by the given path and returns it data as a waveform.
    virtual void Load(const std::string& audio_description, const double offset, const double duration,
                      const std::string& sample_rate) override;

    /// @brief Write waveform data to the file denoted by the given path using FFMPEG process.
    virtual void Save(const std::string& path, const std::string& data, const std::string& sample_rate,
                      const std::string& codec) override;
};
}  // namespace spleeter

#endif  /// SPLEETER_FFMPEG_AUDIO_ADAPTER_H_
