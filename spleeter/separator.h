///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_SEPARATOR_H_
#define SPLEETER_SEPARATOR_H_

#include "spleeter/i_audio_adapter.h"
#include "spleeter/i_separator.h"

#include <memory>
#include <string>

namespace spleeter
{
class Separator : public ISeparator
{
  public:
    explicit Separator(const std::string& configuration, const bool mwf = false);
    virtual ~Separator() = default;

    virtual void Separate(const std::string& waveform) override;
    virtual void SeparateToFile(const std::string& audio_descriptor, const std::string& destination,
                                const std::string& audio_adapter, const double offset, const double duration,
                                const std::string& codec, const std::string bitrate, const std::string& filename_format,
                                const bool& synchronous) override;

    /// @brief Wait for all pending tasks to be finished.
    virtual void Join(const double& timeout);

  private:
    virtual void GetPredictor() const;

    bool mwf_;
    std::unique_ptr<IAudioAdapter> audio_adapter_;
};
}  // namespace spleeter

#endif  /// SPLEETER_SEPARATOR_H_
