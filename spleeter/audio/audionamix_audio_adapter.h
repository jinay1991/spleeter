///
/// @file
/// @brief Contains interface for Audio Adapter
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_AUDIO_AUDIONAMIX_AUDIO_ADAPTER_H_
#define SPLEETER_AUDIO_AUDIONAMIX_AUDIO_ADAPTER_H_

#include "spleeter/audio/i_audio_adapter.h"
#include "spleeter/data_types/audio_properties.h"

#include <wave/file.h>

#include <cstdint>
#include <string>

namespace spleeter
{
/// @brief Audio Adapter to read/write audio files based on Audionamix lib
class AudionamixAudioAdapter : public IAudioAdapter
{
  public:
    /// @brief Constructor.
    AudionamixAudioAdapter();

    /// @brief Destructor.
    ~AudionamixAudioAdapter() = default;

    /// @brief Loads the audio file denoted by the given path and returns it data as a waveform.
    ///
    /// @param path [in]         - Path of the audio file to load data from.
    /// @param offset [in]       - Start offset to load from in seconds.
    /// @param duration [in]     - Duration to load in seconds.
    /// @param sample_rate [in]  - Sample rate to load audio with.
    ///
    /// @returns Loaded data a (waveform, sample_rate) pair.
    Waveform Load(const std::string& path, const double offset, const double duration,
                  const std::int32_t sample_rate) override;

    /// @brief Write waveform data to the file denoted by the given path using FFMPEG process.
    ///
    /// @param path [in]        - Path of the audio file to save data in.
    /// @param data [in]        - Waveform data to write.
    /// @param sample_rate [in] - Sample rate to write file in.
    /// @param codec [in]       - Writing codec to use.
    /// @param bitrate [in]     - Bitrate of the written audio file.
    void Save(const std::string& path, const Waveform& data, const std::int32_t sample_rate, const std::string& codec,
              const std::int32_t bitrate) override;

    /// @brief Provide shape of the Waveform (nb_frames, nb_channels)
    ///
    /// @return Pair of integers (nb_frames, nb_channels)
    AudioProperties GetProperties() const override;

  private:
    /// @brief Loaded Audio Properties
    AudioProperties audio_properties_;
};
}  // namespace spleeter

#endif  /// SPLEETER_AUDIO_AUDIONAMIX_AUDIO_ADAPTER_H_
