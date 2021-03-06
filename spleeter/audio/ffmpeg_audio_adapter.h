///
/// @file
/// @brief Contains implementation of FFMPEG based Audio Adapter
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_AUDIO_FFMPEG_AUDIO_ADAPTER_H
#define SPLEETER_AUDIO_FFMPEG_AUDIO_ADAPTER_H

#include "spleeter/audio/i_audio_adapter.h"
#include "spleeter/datatypes/audio_properties.h"

#ifndef __cplusplus__
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#ifndef __cplusplus__
}
#endif

#include <algorithm>
#include <cstdint>
#include <string>
#include <utility>

namespace spleeter
{
/// @brief An AudioAdapter implementation that use FFMPEG libraries to perform I/O operation for audio processing.
class FfmpegAudioAdapter final : public IAudioAdapter
{
  public:
    /// @brief Constructor.
    FfmpegAudioAdapter();

    /// @brief Loads the audio file denoted by the given path and returns it data as a waveform.
    ///
    /// @param path [in]         - Path of the audio file to load data from.
    /// @param offset [in]       - Start offset to load from in seconds.
    /// @param duration [in]     - Duration to load in seconds.
    /// @param sample_rate [in]  - Sample rate to load audio with.
    ///
    /// @returns Loaded data as waveform
    Waveform Load(const std::string& path,
                  const double offset,
                  const double duration,
                  const std::int32_t sample_rate) override;

    /// @brief Write waveform data to the file denoted by the given path using FFMPEG process.
    ///
    /// @param path [in]        - Path of the audio file to save data in.
    /// @param waveform [in]    - Waveform data to write.
    /// @param sample_rate [in] - Sample rate to write file in.
    /// @param codec [in]       - Writing codec to use.
    /// @param bitrate [in]     - Bitrate of the written audio file.
    void Save(const std::string& path,
              const Waveform& waveform,
              const std::int32_t sample_rate,
              const std::string& codec,
              const std::int32_t bitrate) override;

    /// @brief Provide properties of the Waveform (nb_frames, nb_channels, sample_rate)
    ///
    /// @return audio properties
    AudioProperties GetProperties() const override;

  private:
    /// @brief Loaded Audio Properties
    AudioProperties audio_properties_;
};
}  // namespace spleeter

#endif  /// SPLEETER_AUDIO_FFMPEG_AUDIO_ADAPTER_H
