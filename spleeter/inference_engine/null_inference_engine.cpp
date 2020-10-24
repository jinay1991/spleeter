///
/// @file
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/inference_engine/null_inference_engine.h"

namespace spleeter
{

NullInferenceEngine::NullInferenceEngine(const InferenceEngineParameters& params)
    : results_{params.output_tensor_names.size()}
{
}

void NullInferenceEngine::Init() {}

void NullInferenceEngine::Execute(const Waveform& /* waveform */) {}

void NullInferenceEngine::Shutdown() {}

Waveforms NullInferenceEngine::GetResults() const
{
    return results_;
}

}  // namespace spleeter
