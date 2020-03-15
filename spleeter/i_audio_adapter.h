///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_I_AUDIO_ADAPTER_H_
#define SPLEETER_I_AUDIO_ADAPTER_H_

#include <string>

namespace spleeter
{
class IAudioAdapter
{
  public:
    /// @brief Destructor.
    virtual ~IAudioAdapter() = default;

    /// @brief Loads the audio file denoted by the given path and returns it data as a waveform.
    virtual void Load(const std::string& audio_description, const double offset, const double duration,
                      const std::string& sample_rate) = 0;

    /// @brief Write waveform data to the file denoted by the given path using FFMPEG process.
    virtual void Save(const std::string& path, const std::string& data, const std::string& sample_rate,
                      const std::string& codec) = 0;
};
}  // namespace spleeter

#endif  /// SPLEETER_I_AUDIO_ADAPTER_H_
