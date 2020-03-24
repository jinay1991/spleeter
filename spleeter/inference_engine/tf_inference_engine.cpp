///
/// @file
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/inference_engine/tf_inference_engine.h"
#include "spleeter/logging/logging.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

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
        output_tensor_names = std::vector<std::string>{"strided_slice_18", "strided_slice_38", "strided_slice_48",
                                                       "strided_slice_28", "strided_slice_58"};
    }
    return output_tensor_names;
}
}  // namespace internal

TFInferenceEngine::TFInferenceEngine() : TFInferenceEngine{CLIOptions{}} {}

TFInferenceEngine::TFInferenceEngine(const CLIOptions& cli_options)
    : cli_options_{cli_options},
      bundle_{std::make_shared<tensorflow::SavedModelBundle>()},
      input_tensor_{},
      output_tensors_{},
      output_tensor_names_{internal::GetOutputTensorNames(cli_options_.configuration)},
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

void TFInferenceEngine::Execute() { results_ = InvokeInference(); }

void TFInferenceEngine::Shutdown() {}

Waveforms TFInferenceEngine::InvokeInference() const
{
    auto inputs = std::vector<std::pair<std::string, tensorflow::Tensor>>{{"Placeholder", input_tensor_}};
    auto target_node_names = std::vector<std::string>{};
    auto outputs = std::vector<tensorflow::Tensor>{};
    auto status = bundle_->GetSession()->Run(inputs, output_tensor_names_, target_node_names, &outputs);
    ASSERT_CHECK(status.ok()) << "Unable to run Session";

    SPLEETER_LOG(INFO) << "Successfully received split " << outputs.size() << " waves.";

    /// Extract results
    auto waveforms = Waveforms{};
    std::transform(outputs.begin(), outputs.end(), std::back_inserter(waveforms), [&](const auto& tensor) {
        auto output_data = std::string{};
        //    output_data.resize(input_tensor_.dim_size);

        //    auto tensor_data = tensor.matrix<std::uint8_t>().data();
        //    std::copy(tensor_data, tensor_data + output_data.length(), output_data);

        return output_data;
    });

    return waveforms;
}

void TFInferenceEngine::SetInputWaveform(const Waveform& waveform)
{
    input_tensor_ = tensorflow::Tensor{tensorflow::DT_FLOAT,
                                       tensorflow::TensorShape{static_cast<std::int32_t>(waveform.length()), 2}};
    std::copy(waveform.begin(), waveform.end(), input_tensor_.matrix<float>().data());
}

Waveforms TFInferenceEngine::GetResults() const { return results_; }

std::string TFInferenceEngine::GetModelPath() const
{
    auto config = cli_options_.configuration.substr(cli_options_.configuration.find_first_of(':') + 1);
    return model_dir_ + config;
}

bool TFInferenceEngine::IsVerbosityEnabled() const { return cli_options_.verbose; }

}  // namespace spleeter
