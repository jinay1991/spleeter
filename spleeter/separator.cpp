///
/// @file
/// @brief Contains definitions for Separator class methods
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/separator.h"

#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/audio/ffmpeg_audio_adapter.h"
#include "spleeter/logging/logging.h"

#include <algorithm>
#include <memory>
#include <vector>

#include <experimental/filesystem>

namespace spleeter
{
namespace internal
{
std::vector<std::string> GetWaveformNames(const std::string& configuration)
{
    auto waveform_names = std::vector<std::string>{};
    if (configuration == "spleeter:2stems")
    {
        waveform_names = std::vector<std::string>{"vocal", "accompaniment"};
    }

    else if (configuration == "spleeter:4stems")
    {
        waveform_names = std::vector<std::string>{"vocal", "drums", "bass", "accompaniment"};
    }
    else  // default to "spleeter:5stems"
    {
        waveform_names = std::vector<std::string>{"vocal", "drums", "bass", "piano", "accompaniment"};
    }
    return waveform_names;
}
}  // namespace internal

Separator::Separator(const InferenceEngineParameters& inference_engine_param, const bool mwf)
    : mwf_{mwf},
      audio_adapter_{std::make_unique<AudionamixAudioAdapter>()},
      inference_engine_{},
      waveform_name_{internal::GetWaveformNames(inference_engine_param.configuration)}
{
    inference_engine_.SelectInferenceEngine(InferenceEngineType::kTensorFlowLite, inference_engine_param);
    inference_engine_.Init();
}

Waveforms Separator::Separate(const Waveform& waveform)
{
    inference_engine_.Execute(waveform);
    return inference_engine_.GetResults();
}

void Separator::SeparateToFile(const std::string& audio_descriptor,
                               const std::string& destination,
                               const std::string& /*audio_adapter*/,
                               const double offset,
                               const double duration,
                               const std::string& codec,
                               const std::int32_t bitrate,
                               const std::string& /*filename_format*/,
                               const bool /*synchronous*/)
{
    const auto waveform = audio_adapter_->Load(audio_descriptor, offset, duration, -1);

    auto waveforms = Separate(waveform);

    for (auto idx = 0U; idx < waveforms.size(); ++idx)
    {
        if (!std::experimental::filesystem::exists(destination))
        {
            std::experimental::filesystem::create_directory(destination);
        }
        const auto path = destination + "/" + waveform_name_[idx] + "." + codec;
        audio_adapter_->Save(path, waveforms[idx], -1, codec, bitrate);
    }
}

}  // namespace spleeter
