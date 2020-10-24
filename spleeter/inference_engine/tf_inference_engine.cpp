///
/// @file
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/inference_engine/tf_inference_engine.h"

#include "spleeter/logging/logging.h"

#include <unordered_set>

namespace spleeter
{
namespace
{
/// @brief Converts tensorflow::Tensor to waveform (aka cv::Mat)
///
/// @param tensor[in] tensorflow::Tensor in [NxHxWxC form]
///
/// @return Equivalent waveform for given tensorflow::Tensor by copying contents.
Waveform ConvertToWaveform(const tensorflow::Tensor& tensor)
{
    tensorflow::Tensor tensor_matrix = tensor;
    const auto rows = tensor_matrix.dims() > 1 ? static_cast<std::int32_t>(tensor_matrix.dim_size(1)) : 1;
    const auto cols = tensor_matrix.dims() > 2 ? static_cast<std::int32_t>(tensor_matrix.dim_size(2)) : 1;
    const auto channels = tensor_matrix.dims() > 3 ? static_cast<std::int32_t>(tensor_matrix.dim_size(3)) : 1;
    const auto size = rows * cols * channels;
    auto* tensor_ptr = tensor_matrix.flat<float>().data();
    Waveform waveform{};
    waveform.nb_frames = rows;
    waveform.nb_channels = channels;
    std::copy(tensor_ptr, tensor_ptr + size, std::back_inserter(waveform.data));
    return waveform;
}

/// @brief Converts waveform to tensorflow::Tensor
///
/// @param waveform[in] waveform
///
/// @return Equivalent tensorflow::Tensor for given waveform by copying the its contents to tensor.
tensorflow::Tensor ConvertToTensor(const Waveform& waveform)
{
    tensorflow::Tensor tensor{tensorflow::DT_FLOAT, tensorflow::TensorShape{waveform.nb_frames, waveform.nb_channels}};
    std::copy(waveform.data.begin(), waveform.data.end(), tensor.matrix<float>().data());
    return tensor;
}
}  // namespace

TFInferenceEngine::TFInferenceEngine(const InferenceEngineParameters& params)
    : bundle_{std::make_shared<tensorflow::SavedModelBundle>()},
      input_tensor_{},
      input_tensor_name_{params.input_tensor_name},
      output_tensors_{},
      output_tensor_names_{params.output_tensor_names},
      model_path_{params.model_path}
{
}

void TFInferenceEngine::Init()
{
    tensorflow::SessionOptions session_options{};
    tensorflow::RunOptions run_options{};
    std::unordered_set<std::string> tags{"serve"};

    const auto ret = tensorflow::LoadSavedModel(session_options, run_options, model_path_, tags, bundle_.get());
    ASSERT_CHECK(ret.ok()) << "Failed to load saved model '" << model_path_ << "', (Message: " << ret.error_message()
                           << ")";

    SPLEETER_LOG(INFO) << "Successfully loaded saved model from '" << model_path_ << "'.";
}

void TFInferenceEngine::Execute(const Waveform& waveform)
{
    UpdateInput(waveform);
    UpdateTensors();
    UpdateOutputs();
}

void TFInferenceEngine::Shutdown() {}

Waveforms TFInferenceEngine::GetResults() const
{
    return results_;
}

void TFInferenceEngine::UpdateInput(const Waveform& waveform)
{
    input_tensor_ = ConvertToTensor(waveform);
}

void TFInferenceEngine::UpdateTensors()
{
    const std::vector<std::pair<std::string, tensorflow::Tensor>> inputs{{input_tensor_name_, input_tensor_}};
    const std::vector<std::string> target_node_names{};

    const auto ret = bundle_->GetSession()->Run(inputs, output_tensor_names_, target_node_names, &output_tensors_);
    ASSERT_CHECK(ret.ok()) << "Unable to run Session, (Message: " << ret.error_message() << ")";

    SPLEETER_LOG(INFO) << "Successfully received results " << output_tensors_.size() << " outputs.";
}

void TFInferenceEngine::UpdateOutputs()
{
    std::transform(
        output_tensors_.cbegin(), output_tensors_.cend(), std::back_inserter(results_), [](auto const& tensor) {
            return ConvertToWaveform(tensor);
        });
}

}  // namespace spleeter
