///
/// @file
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/inference_engine/tf_inference_engine.h"
#include "spleeter/logging/logging.h"

#include <memory>
#include <string>
#include <unordered_set>

namespace spleeter
{
TFInferenceEngine::TFInferenceEngine() : TFInferenceEngine{CLIOptions{}} {}

TFInferenceEngine::TFInferenceEngine(const CLIOptions& cli_options)
    : cli_options_{cli_options},
      bundle_{std::make_shared<tensorflow::SavedModelBundle>()},
      input_tensor_{},
      output_tensors_{},
      output_tensor_names_{"strided_slice_18", "strided_slice_38", "strided_slice_48", "strided_slice_28",
                           "strided_slice_58"}
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
    SetInputWaveform(Waveform{});

    results_ = InvokeInference();
}

void TFInferenceEngine::Shutdown() {}

Waveforms TFInferenceEngine::InvokeInference() const
{
    auto status =
        bundle_->session->Run({{"Placeholder", input_tensor_}}, output_tensor_names_, {}, &output_tensors_, nullptr);
    ASSERT_CHECK(status.ok()) << "Unable to run Session";

    /// Extract results
    auto waveforms = Waveforms{};
    std::transform(output_tensors_.begin(), output_tensors_.end(), std::back_inserter(waveforms),
                   [&](const auto& tensor) {
                       auto output_data = std::string{};
                       output_data.resize(input_tensor_.dim_size);

                       auto tensor_data = tensor.matrix<std::uint8_t>().data();
                       std::copy(tensor_data, tensor_data + output_data.length(), output_data);

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

std::string TFInferenceEngine::GetModelPath() const { return "data/model.t"; }

bool TFInferenceEngine::IsVerbosityEnabled() const { return cli_options_.verbose; }

}  // namespace spleeter
