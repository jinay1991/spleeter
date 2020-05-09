///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/inference_engine/inference_engine.h"

#include "spleeter/inference_engine/tf_inference_engine.h"
#include "spleeter/inference_engine/tflite_inference_engine.h"
#include "spleeter/inference_engine/torch_inference_engine.h"

namespace spleeter
{
InferenceEngine::InferenceEngine() {}

void InferenceEngine::SelectInferenceEngine(const InferenceEngineType& inference_engine,
                                            const std::string& configuration)
{
    switch (inference_engine)
    {
        default:
        case InferenceEngineType::kTensorFlowLite:
            inference_engine_ = std::make_unique<TFLiteInferenceEngine>(configuration);
            break;
        case InferenceEngineType::kTensorFlow:
            inference_engine_ = std::make_unique<TFInferenceEngine>(configuration);
            break;
        case InferenceEngineType::kTorch:
            inference_engine_ = std::make_unique<TorchInferenceEngine>(configuration);
            break;
    }
}

void InferenceEngine::Init()
{
    inference_engine_->Init();
}

void InferenceEngine::Execute()
{
    inference_engine_->Execute();
}

void InferenceEngine::Shutdown()
{
    inference_engine_->Shutdown();
}

void InferenceEngine::SetInputWaveform(const Waveform& waveform,
                                       const std::int32_t nb_frames,
                                       const std::int32_t nb_channels)
{
    inference_engine_->SetInputWaveform(waveform, nb_frames, nb_channels);
}

Waveforms InferenceEngine::GetResults() const
{
    return inference_engine_->GetResults();
}

InferenceEngineType InferenceEngine::GetType() const
{
    return inference_engine_->GetType();
};

std::string InferenceEngine::GetConfiguration() const
{
    return inference_engine_->GetConfiguration();
}
}  // namespace spleeter
