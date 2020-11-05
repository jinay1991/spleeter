///
/// @file
/// @copyright Copyright (c) 2020. MIT License
///
#ifndef SPLEETER_INFERENCE_ENGINE_TFLITE_INFERENCE_ENGINE_H
#define SPLEETER_INFERENCE_ENGINE_TFLITE_INFERENCE_ENGINE_H

#include "spleeter/datatypes/inference_engine.h"
#include "spleeter/inference_engine/i_inference_engine.h"

#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>

#include <memory>

namespace spleeter
{
/// @brief TFLite Inference Engine class
class TFLiteInferenceEngine final : public IInferenceEngine
{
  public:
    /// @brief Constructor
    explicit TFLiteInferenceEngine(const InferenceEngineParameters& params);

    /// @brief Initialise TFLite Inference Engine
    void Init() override;

    /// @brief Execute Inference with TFLite Inference Engine
    /// @param waveform [in] - Waveform to be split
    void Execute(const Waveform& waveform) override;

    /// @brief Release TFLite Inference Engine
    void Shutdown() override;

    /// @brief Obtain Results for provided input waveform
    ///
    /// @return List of waveforms (split waveforms)
    Waveforms GetResults() const override;

  private:
    /// @brief Updates Input Tensor by copying waveform to input_tensor
    /// @param waveform [in] - Waveform to be split
    void UpdateInput(const Waveform& waveform);

    /// @brief Updates Output Tensors by running the tensorflow session
    void UpdateTensors();

    /// @brief Converts output_tensors to Waveforms results
    void UpdateOutputs();

    /// @brief Resizes Input Tensor based on the provided waveform shape
    void ResizeInputTensor(const Waveform& waveform);

    /// @brief Resizes Output Tensor based on the provided waveform shape
    void ResizeOutputTensor(const Waveform& waveform);

    /// @brief Resizes Tensor at given index to the provided dimensions
    /// @param tensor_index[in] Tensor Index to resize
    /// @param dims[in] Requested dimensions for the Tensor
    void ResizeTensor(const std::int32_t tensor_index, const std::vector<std::int32_t> dims);

    /// @brief TFLite Model Buffer Instance
    std::unique_ptr<tflite::FlatBufferModel> model_;

    /// @brief TFLite Model Interpreter instance
    std::unique_ptr<tflite::Interpreter> interpreter_;

    /// @brief Input Tensor name
    const std::string input_tensor_name_;

    /// @brief Output Tensors names
    const std::vector<std::string> output_tensor_names_;

    /// @brief TFLite Inference output tensor indices
    std::vector<std::int32_t> output_tensor_indicies_;

    /// @brief Model root directory
    const std::string model_path_;

    /// @brief Output Tensors saved as Waveforms
    Waveforms results_;
};

}  // namespace spleeter
#endif  /// SPLEETER_INFERENCE_ENGINE_TFLITE_INFERENCE_ENGINE_H
