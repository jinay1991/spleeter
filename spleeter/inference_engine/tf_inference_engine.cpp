///
/// @file
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/inference_engine/tf_inference_engine.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "spleeter/logging/logging.h"

namespace spleeter
{
namespace internal
{
std::vector<std::string> GetOutputTensorNames(const std::string& configuration)
{
    auto output_tensor_names = std::vector<std::string>{};
    if (configuration == "spleeter:2stems")
    {
        output_tensor_names = std::vector<std::string>{"strided_slice_13", "strided_slice_23"};
    }

    else if (configuration == "spleeter:4stems")
    {
        output_tensor_names =
            std::vector<std::string>{"strided_slice_13", "strided_slice_23", "strided_slice_33", "strided_slice_43"};
    }
    else  // default to "spleeter:5stems"
    {
        output_tensor_names = std::vector<std::string>{
            "strided_slice_18", "strided_slice_38", "strided_slice_48", "strided_slice_28", "strided_slice_58"};
    }
    return output_tensor_names;
}
}  // namespace internal

TFInferenceEngine::TFInferenceEngine() : TFInferenceEngine{"spleeter:5stems"} {}

TFInferenceEngine::TFInferenceEngine(const std::string& configuration)
    : configuration_{configuration},
      bundle_{std::make_shared<tensorflow::SavedModelBundle>()},
      input_tensor_{},
      output_tensors_{},
      output_tensor_names_{internal::GetOutputTensorNames(configuration_)},
      model_dir_{"external/models/"}
{
}

void TFInferenceEngine::Init()
{
    auto session_options = tensorflow::SessionOptions{};
    auto run_options = tensorflow::RunOptions{};
    auto tags = std::unordered_set<std::string>{{"serve"}};

    auto ret = tensorflow::LoadSavedModel(session_options, run_options, GetModelPath(), tags, bundle_.get());
    ASSERT_CHECK(ret.ok()) << "Failed to load saved model";
}

void TFInferenceEngine::Execute()
{
    results_ = InvokeInference();
}

void TFInferenceEngine::Shutdown() {}

Waveforms TFInferenceEngine::InvokeInference() const
{
    auto inputs = std::vector<std::pair<std::string, tensorflow::Tensor>>{{"Placeholder", input_tensor_}};
    auto target_node_names = std::vector<std::string>{};
    auto outputs = std::vector<tensorflow::Tensor>{};
    auto status = bundle_->GetSession()->Run(inputs, output_tensor_names_, target_node_names, &outputs);
    ASSERT_CHECK(status.ok()) << "Unable to run Session, (Returned: " << status.ok() << ")";

    SPLEETER_LOG(INFO) << "Successfully received split " << outputs.size() << " waves.";

    /// Extract results
    auto waveforms = Waveforms{};
    std::transform(
        outputs.begin(), outputs.end(), std::back_inserter(waveforms), [&](const tensorflow::Tensor& tensor) {
            auto waveform = Waveform{};
            waveform.resize(input_tensor_.dim_size(0) * input_tensor_.dim_size(1));

            auto tensor_dataptr = tensor.matrix<float>().data();
            std::copy(tensor_dataptr, tensor_dataptr + waveform.size(), waveform.begin());
            return waveform;
        });

    return waveforms;
}

void TFInferenceEngine::SetInputWaveform(const Waveform& waveform,
                                         const std::int32_t nb_frames,
                                         const std::int32_t nb_channels)
{
    input_tensor_ = tensorflow::Tensor{tensorflow::DT_FLOAT, tensorflow::TensorShape{nb_frames, nb_channels}};
    std::copy(waveform.begin(), waveform.end(), input_tensor_.matrix<float>().data());
}

Waveforms TFInferenceEngine::GetResults() const
{
    return results_;
}

std::string TFInferenceEngine::GetModelPath() const
{
    auto config = configuration_.substr(configuration_.find_first_of(':') + 1);
    return model_dir_ + config;
}

InferenceEngineType TFInferenceEngine::GetType() const
{
    return InferenceEngineType::kTensorFlow;
}

std::string TFInferenceEngine::GetConfiguration() const
{
    return configuration_;
}

}  // namespace spleeter
