///
/// @file
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/inference_engine/tflite_inference_engine.h"

#include "spleeter/logging/logging.h"

#include <tensorflow/lite/builtin_op_data.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/optional_debug_tools.h>

namespace spleeter
{

TFLiteInferenceEngine::TFLiteInferenceEngine(const InferenceEngineParameters& params)
    : model_path_{params.model_path}, results_{}
{
}

void TFLiteInferenceEngine::Init()
{
    model_ = tflite::FlatBufferModel::BuildFromFile(model_path_.c_str());
    ASSERT_CHECK(model_) << "Failed to read model " << model_path_;
    model_->error_reporter();

    tflite::ops::builtin::BuiltinOpResolver resolver{};

    tflite::InterpreterBuilder(*model_, resolver)(&interpreter_);
    ASSERT_CHECK(interpreter_) << "Failed to construct interpreter";

    ASSERT_CHECK_EQ(interpreter_->AllocateTensors(), TfLiteStatus::kTfLiteOk) << "Failed to allocate tensors!";

    SPLEETER_LOG(INFO) << "Successfully loaded tflite model from '" << model_path_ << "'.";
}

void TFLiteInferenceEngine::Execute(const Waveform& waveform)
{
    UpdateInput(waveform);
    UpdateTensors();
    UpdateOutputs();
}

void TFLiteInferenceEngine::Shutdown() {}

Waveforms TFLiteInferenceEngine::GetResults() const
{
    return results_;
}

void TFLiteInferenceEngine::UpdateInput(const Waveform& waveform)
{
    const auto input = interpreter_->inputs()[0];

    TfLiteIntArray* dims = interpreter_->tensor(input)->dims;
    const auto wanted_height = dims->data[1];
    const auto wanted_width = dims->data[2];
    const auto wanted_channels = dims->data[3];
    const auto mean = 1.0;
    const auto stddev = 0.0;

    /// @todo Update inputs for inference
}

void TFLiteInferenceEngine::UpdateTensors()
{
    const auto ret = interpreter_->Invoke();
    ASSERT_CHECK_EQ(ret, TfLiteStatus::kTfLiteOk) << "Failed to invoke tflite!";
}

void TFLiteInferenceEngine::UpdateOutputs()
{
    const auto output = interpreter_->outputs()[0];
    const auto output_dims = interpreter_->tensor(output)->dims;
    const auto output_size = output_dims->data[output_dims->size - 1];

    results_.resize(output_size);

    /// @todo Update results with inference output
}

}  // namespace spleeter
