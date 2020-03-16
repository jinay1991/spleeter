///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/separator.h"
#include "spleeter/ffmpeg_audio_adapter.h"

namespace spleeter
{
Separator::Separator(const std::string& configuration, const bool mwf)
    : mwf_{mwf}, audio_adapter_{std::make_unique<FfmpegAudioAdapter>()}, sample_rate_{44100}
{
}

std::vector<Waveform> Separator::Separate(const Waveform& waveform) { return std::vector<Waveform>{}; }

void Separator::SeparateToFile(const std::string& audio_descriptor, const std::string& destination,
                               const std::string& audio_adapter, const double offset, const double duration,
                               const std::string& codec, const std::string bitrate, const std::string& filename_format,
                               const bool& synchronous)
{
    auto waveform = audio_adapter_->Load(audio_descriptor, offset, duration, sample_rate_);
}
void Separator::Join(const double& timeout) {}
void Separator::GetPredictor() const {};
}  // namespace spleeter
