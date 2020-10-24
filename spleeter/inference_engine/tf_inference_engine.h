///
/// @file
/// @copyright Copyright (c) 2020. MIT License
///
#ifndef SPLEETER_INFERENCE_ENGINE_TF_INFERENCE_ENGINE_H
#define SPLEETER_INFERENCE_ENGINE_TF_INFERENCE_ENGINE_H

#include "spleeter/datatypes/inference_engine.h"
#include "spleeter/inference_engine/i_inference_engine.h"

#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/saved_model/loader.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace spleeter
{
/// @brief TensorFlow Inference Engine class
class TFInferenceEngine final : public IInferenceEngine
{
  public:
    /// @brief Constructor
    /// @param params[in] Inference Engine Parameters such as model input/output node names
    explicit TFInferenceEngine(const InferenceEngineParameters& params);

    /// @brief Initialise TensorFlow Inference Engine
    void Init() override;

    /// @brief Execute Inference with TensorFlow Inference Engine
    /// @param waveform [in] - Waveform to be split
    void Execute(const Waveform& waveform) override;

    /// @brief Release TensorFlow Inference Engine
    void Shutdown() override;

    /// @brief Provide results in terms of Matrix
    /// @return List of waveforms (split waveforms)
    Waveforms GetResults() const override;

  private:
    /// @brief Updates Input Tensor by copying waveform to input_tensor
    /// @param waveform [in] - Waveform to be split
    void UpdateInput(const Waveform& waveform);

    /// @brief Updates Output Tensors by running the tensorflow session
    void UpdateTensors();

    /// @brief Converts output_tensors to cv::Mat results
    void UpdateOutputs();

    /// @brief Saved Model bundle
    std::shared_ptr<tensorflow::SavedModelBundle> bundle_;

    /// @brief Input Tensor
    tensorflow::Tensor input_tensor_;

    /// @brief Input Tensor name
    const std::string input_tensor_name_;

    /// @brief Output Tensors
    std::vector<tensorflow::Tensor> output_tensors_;

    /// @brief Output Tensors names
    const std::vector<std::string> output_tensor_names_;

    /// @brief Model root directory
    const std::string model_path_;

    /// @brief Output Tensors saved as cv::Mat
    Waveforms results_;
};

}  // namespace spleeter

#endif  /// SPLEETER_INFERENCE_ENGINE_TF_INFERENCE_ENGINE_H
