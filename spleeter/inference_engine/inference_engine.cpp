///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/inference_engine/inference_engine.h"
// #include "spleeter/inference_engine/tf_inference_engine.h"
#include "spleeter/inference_engine/tflite_inference_engine.h"

#include <memory>

namespace spleeter
{
InferenceEngine::InferenceEngine() : InferenceEngine{CLIOptions{}} {}
InferenceEngine::InferenceEngine(const CLIOptions& cli_options) : cli_options_{cli_options} {}

void InferenceEngine::SelectInferenceEngine(const InferenceEngineType& inference_engine)
{
    switch (inference_engine)
    {
        default:
        case InferenceEngineType::kTensorFlowLite:
            inference_engine_ = std::make_unique<TFLiteInferenceEngine>();
            break;
        // case InferenceEngineType::kTensorFlow:
        //     inference_engine_ = std::make_unique<TFInferenceEngine>();
        //     break;
    }
}

void InferenceEngine::Init() { inference_engine_->Init(); }

void InferenceEngine::Execute() { inference_engine_->Execute(); }

void InferenceEngine::Shutdown() { inference_engine_->Shutdown(); }

void InferenceEngine::SetInputWaveform(const Waveform& waveform) { inference_engine_->SetInputWaveform(waveform); }

Waveforms InferenceEngine::GetResults() const { return inference_engine_->GetResults(); }

}  // namespace spleeter
