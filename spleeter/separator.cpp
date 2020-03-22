///
/// @file
/// @brief Contains definitions for Separator class methods
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/separator.h"
#include "spleeter/audio/ffmpeg_audio_adapter.h"

namespace spleeter
{
Separator::Separator(const std::string& configuration, const bool mwf)
    : mwf_{mwf}, audio_adapter_{std::make_unique<FfmpegAudioAdapter>()}, sample_rate_{44100}
{
}

std::vector<Waveform> Separator::Separate(const Waveform& waveform)
{
    /// pass to TFLite for separating waveform based on configurations
    /// -2. Export *.ckpt to *.pb
    /// -1. Freeze *.pb
    /// 0. Convert *.pb to *.tflite
    /// 1. Open TFLite Model
    /// 2. Extract Input/Output Node
    /// 3. Preprocess waveform
    /// 4. Postprocess resultant waveform
    return std::vector<Waveform>{};
}

void Separator::SeparateToFile(const std::string& audio_descriptor, const std::string& destination,
                               const std::string& audio_adapter, const double offset, const double duration,
                               const std::string& codec, const std::string bitrate, const std::string& filename_format,
                               const bool& synchronous)
{
    auto waveforms_sample_rate_tuple = audio_adapter_->Load(audio_descriptor, offset, duration, sample_rate_);

    // auto waveforms = Separate(waveforms_sample_rate_tuple.first);
}
void Separator::Join(const double& timeout) {}
void Separator::GetPredictor() const {};
}  // namespace spleeter
