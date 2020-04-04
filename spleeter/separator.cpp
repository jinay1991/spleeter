///
/// @file
/// @brief Contains definitions for Separator class methods
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/separator.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/audio/ffmpeg_audio_adapter.h"
#include "spleeter/logging/logging.h"

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

Separator::Separator(const std::string& configuration, const bool mwf)
    : mwf_{mwf},
      audio_adapter_{std::make_unique<AudionamixAudioAdapter>()},
      inference_engine_{},
      waveform_name_{internal::GetWaveformNames(configuration)}
{
    inference_engine_.SelectInferenceEngine(InferenceEngineType::kTensorFlow, configuration);
    inference_engine_.Init();
}

Waveforms Separator::Separate(const Waveform& waveform, const AudioProperties& properties)
{
    inference_engine_.SetInputWaveform(waveform, properties.nb_frames, properties.nb_channels);

    inference_engine_.Execute();

    auto waveforms = inference_engine_.GetResults();
    return waveforms;
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
    auto waveform = audio_adapter_->Load(audio_descriptor, offset, duration, -1);

    auto waveforms = Separate(waveform, audio_adapter_->GetProperties());

    for (auto idx = 0U; idx < waveforms.size(); ++idx)
    {
        if (!std::experimental::filesystem::exists(destination))
        {
            std::experimental::filesystem::create_directory(destination);
        }
        auto path = destination + "/" + waveform_name_[idx] + "." + codec;
        audio_adapter_->Save(path, waveforms[idx], -1, codec, bitrate);
    }
}

}  // namespace spleeter
