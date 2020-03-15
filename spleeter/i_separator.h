///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_I_SEPARATOR_H_
#define SPLEETER_I_SEPARATOR_H_

#include <string>

namespace spleeter
{
class ISeparator
{
  public:
    virtual ~ISeparator() = default;

    virtual void Separate(const std::string& waveform) = 0;
    virtual void SeparateToFile(const std::string& audio_descriptor, const std::string& destination,
                                const std::string& audio_adapter, const double offset, const double duration,
                                const std::string& codec, const std::string bitrate, const std::string& filename_format,
                                const bool& synchronous) = 0;
};
}  // namespace spleeter

#endif  /// SPLEETER_I_SEPARATOR_H_
