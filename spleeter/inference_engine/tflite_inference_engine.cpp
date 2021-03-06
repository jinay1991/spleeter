///
/// @file
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/inference_engine/tflite_inference_engine.h"

#include "flatbuffers/flatbuffers.h"
#include "spleeter/logging/logging.h"

#include <tensorflow/lite/builtin_op_data.h>
#include <tensorflow/lite/c/common.h>
#include <tensorflow/lite/kernels/kernel_util.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/optional_debug_tools.h>

#include <iomanip>
#include <ostream>

namespace spleeter
{
namespace
{
/// @brief Converts TfLiteTensor to waveform
///
/// @param tensor[in] TfLiteTensor in [NxHxWxC form]
///
/// @return Equivalent waveform for given TfLiteTensor by copying contents.
Waveform ConvertToWaveform(const TfLiteTensor* tensor)
{
    const TfLiteIntArray* tensor_dims = tensor->dims;
    const auto samples = tensor_dims->size > 0 ? static_cast<std::int32_t>(tensor_dims->data[0]) : 1;
    const auto channels = tensor_dims->size > 1 ? static_cast<std::int32_t>(tensor_dims->data[1]) : 1;
    const auto size = samples * channels;
    const float* tensor_ptr = reinterpret_cast<const float*>(tensor->data.raw);
    Waveform waveform{};
    waveform.nb_frames = samples;
    waveform.nb_channels = channels;
    std::copy(tensor_ptr, tensor_ptr + size, std::back_inserter(waveform.data));
    return waveform;
}

}  // namespace

inline std::ostream& operator<<(std::ostream& os, const TfLiteIntArray* v)
{
    if (!v)
    {
        os << " (null)";
        return os;
    }
    for (int k = 0; k < v->size; k++)
    {
        os << " " << std::dec << v->data[k];
    }
    return os;
}

TFLiteInferenceEngine::TFLiteInferenceEngine(const InferenceEngineParameters& params)
    : input_tensor_name_{params.input_tensor_name},
      output_tensor_names_{params.output_tensor_names},
      output_tensor_indicies_{},
      model_path_{params.model_path},
      results_{}
{
}

void TFLiteInferenceEngine::Init()
{
    model_ = tflite::FlatBufferModel::BuildFromFile(model_path_.c_str());
    CHECK(model_) << "Failed to read model " << model_path_;
    model_->error_reporter();

    tflite::ops::builtin::BuiltinOpResolver resolver{};
    tflite::InterpreterBuilder(*model_, resolver)(&interpreter_);
    CHECK(interpreter_) << "Failed to construct interpreter";

    CHECK_EQ(interpreter_->AllocateTensors(), TfLiteStatus::kTfLiteOk) << "Failed to allocate tensors!";

    LOG(INFO) << "Successfully loaded tflite model from '" << model_path_ << "'.";
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
    ResizeInputTensor(waveform);
    ResizeOutputTensor(waveform);
    CHECK_EQ(interpreter_->AllocateTensors(), TfLiteStatus::kTfLiteOk) << "Failed to allocate tensors!";

    float* tensor_ptr = interpreter_->typed_input_tensor<float>(0U);
    std::copy(waveform.data.cbegin(), waveform.data.cend(), tensor_ptr);

    LOG(INFO) << "Successfully loaded input waveform!!";
}

void TFLiteInferenceEngine::UpdateTensors()
{
    const auto ret = interpreter_->Invoke();
    CHECK_EQ(ret, TfLiteStatus::kTfLiteOk) << "Failed to invoke tflite!";

    output_tensor_indicies_ = interpreter_->outputs();
    LOG(INFO) << "Successfully received results " << output_tensor_indicies_.size() << " outputs.";
}

void TFLiteInferenceEngine::UpdateOutputs()
{
    std::transform(output_tensor_indicies_.cbegin(),
                   output_tensor_indicies_.cend(),
                   std::back_inserter(results_),
                   [this](const auto& tensor_index) {
                       const TfLiteTensor* tensor = interpreter_->tensor(tensor_index);
                       return ConvertToWaveform(tensor);
                   });
}

void TFLiteInferenceEngine::ResizeInputTensor(const Waveform& waveform)
{
    const auto input_tensor_indicies = interpreter_->inputs();
    CHECK_EQ(input_tensor_indicies.size(), 1) << "Model has more than one input tensor";

    ResizeTensor(input_tensor_indicies.at(0U), {waveform.nb_frames, waveform.nb_channels});
}

void TFLiteInferenceEngine::ResizeOutputTensor(const Waveform& waveform)
{
    const auto output_tensor_indicies = interpreter_->outputs();
    CHECK_GT(output_tensor_indicies.size(), 0) << "Model has no output tensor(s)";

    for (auto tensor_index : output_tensor_indicies)
    {
        ResizeTensor(tensor_index, {waveform.nb_frames, waveform.nb_channels});
    }
}

void TFLiteInferenceEngine::ResizeTensor(const std::int32_t tensor_index, const std::vector<std::int32_t> dims)
{
    TfLiteTensor* tensor = interpreter_->tensor(tensor_index);
    tensor->data.raw = nullptr;
    tensor->allocation_type = TfLiteAllocationType::kTfLiteDynamic;

    CHECK_EQ(interpreter_->ResizeInputTensor(tensor_index, dims), TfLiteStatus::kTfLiteOk);
}
}  // namespace spleeter
