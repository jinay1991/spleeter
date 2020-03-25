///
/// @file
/// @brief Contains definitions for Separator class methods
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/separator.h"
#include "spleeter/audio/ffmpeg_audio_adapter.h"
#include "spleeter/logging/logging.h"

#include <algorithm>
#include <memory>
#include <vector>

namespace spleeter
{
Separator::Separator(const std::string& configuration, const bool mwf)
    : mwf_{mwf}, audio_adapter_{std::make_unique<FfmpegAudioAdapter>()}, sample_rate_{44100}
{
    /// Default to TFInferenceEngine
    inference_engine_.SelectInferenceEngine(InferenceEngineType::kTensorFlow);
    inference_engine_.Init();
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
    inference_engine_.SetInputWaveform(waveform, static_cast<std::int32_t>(waveform.size()), 2);

    inference_engine_.Execute();

    auto waveforms = inference_engine_.GetResults();
    return waveforms;
}

void Separator::SeparateToFile(const std::string& audio_descriptor, const std::string& destination,
                               const std::string& audio_adapter, const double offset, const double duration,
                               const std::string& codec, const std::string bitrate, const std::string& filename_format,
                               const bool& synchronous)
{
    auto waveform_sample_rate_pair = audio_adapter_->Load(audio_descriptor, offset, duration, sample_rate_);
    auto waveform = waveform_sample_rate_pair.first;
    auto sample_rate = waveform_sample_rate_pair.second;

    auto waveforms = Separate(waveform);

    std::for_each(waveforms.begin(), waveforms.end(),
                  [&](const auto& waveform) { SPLEETER_LOG(INFO) << "Saving waveforms..."; });
}
void Separator::Join(const double& timeout) {}
void Separator::GetPredictor() const {};
}  // namespace spleeter
