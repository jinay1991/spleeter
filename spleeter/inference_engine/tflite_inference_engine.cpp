///
/// @file
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/inference_engine/tflite_inference_engine.h"

#include "flatbuffers/flatbuffers.h"
#include "spleeter/logging/logging.h"

#include <tensorflow/lite/builtin_op_data.h>
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
    const auto rows = tensor_dims->size > 1 ? static_cast<std::int32_t>(tensor_dims->data[1]) : 1;
    const auto cols = tensor_dims->size > 2 ? static_cast<std::int32_t>(tensor_dims->data[2]) : 1;
    const auto channels = tensor_dims->size > 3 ? static_cast<std::int32_t>(tensor_dims->data[3]) : 1;
    const auto size = rows * cols * channels;
    const float* tensor_ptr = reinterpret_cast<const float*>(tensor->data.raw);
    Waveform waveform{};
    waveform.nb_frames = rows;
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
    const auto input_tensor_indicies = interpreter_->inputs();
    ASSERT_CHECK_EQ(input_tensor_indicies.size(), 1) << "Provided model has more than 1 input";

    TfLiteTensor* input_tensor = interpreter_->tensor(input_tensor_indicies.at(0U));
    ASSERT_CHECK_EQ(input_tensor->name, input_tensor_name_)
        << "Received input tensor name does not match with input tensor from graph.";

    interpreter_->ResizeInputTensor(input_tensor_indicies.at(0U), {waveform.nb_frames, waveform.nb_channels});
    ASSERT_CHECK_EQ(interpreter_->AllocateTensors(), TfLiteStatus::kTfLiteOk) << "Failed to allocate tensors!";

    float* tensor_ptr = interpreter_->typed_tensor<float>(input_tensor_indicies.at(0U));
    std::copy(waveform.data.cbegin(), waveform.data.cend(), tensor_ptr);

    SPLEETER_LOG(INFO) << "Successfully loaded input waveform!!";
}

void TFLiteInferenceEngine::UpdateTensors()
{
    const auto ret = interpreter_->Invoke();
    ASSERT_CHECK_EQ(ret, TfLiteStatus::kTfLiteOk) << "Failed to invoke tflite!";

    output_tensor_indicies_ = interpreter_->outputs();
    SPLEETER_LOG(INFO) << "Successfully received results " << output_tensor_indicies_.size() << " outputs.";
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

}  // namespace spleeter
