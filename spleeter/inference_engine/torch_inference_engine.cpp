///
/// @file
/// @copyright Copyright (c) 2020. MIT License.
///
#include "spleeter/inference_engine/torch_inference_engine.h"

#include "spleeter/logging/logging.h"

#include <torch/torch.h>

namespace spleeter
{
TorchInferenceEngine::TorchInferenceEngine() : TorchInferenceEngine{"spleeter:2stems"} {}

TorchInferenceEngine::TorchInferenceEngine(const std::string& configuration) : configuration_{configuration} {}

void TorchInferenceEngine::Init()
{
    torch::Tensor tensor = torch::eye(3);
    SPLEETER_LOG(INFO) << "Successfully created tensor: " << tensor;
}

void TorchInferenceEngine::Execute() {}

void TorchInferenceEngine::Shutdown() {}

void TorchInferenceEngine::SetInputWaveform(const Waveform& waveform,
                                            const std::int32_t nb_frames,
                                            const std::int32_t nb_channels)
{
}

Waveforms TorchInferenceEngine::GetResults() const
{
    Waveforms results{};
    results.resize(5U);
    return results;
}

InferenceEngineType TorchInferenceEngine::GetType() const
{
    return InferenceEngineType::kTorch;
}

std::string TorchInferenceEngine::GetConfiguration() const
{
    return configuration_;
}

}  // namespace spleeter