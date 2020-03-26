///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_INFERENCE_ENGINE_TF_INFERENCE_ENGINE_H_
#define SPLEETER_INFERENCE_ENGINE_TF_INFERENCE_ENGINE_H_

#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/inference_engine/i_inference_engine.h"

#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/saved_model/loader.h>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace spleeter
{
/// @brief TensorFlow Inference Engine class
class TFInferenceEngine : public IInferenceEngine
{
  public:
    /// @brief Default Constructor
    TFInferenceEngine();

    /// @brief Constructor
    ///
    /// @param configuration [in] - configurations
    explicit TFInferenceEngine(const std::string& configuration);

    /// @brief Destructor
    ~TFInferenceEngine() = default;

    /// @brief Initialise TensorFlow Inference Engine
    void Init() override;

    /// @brief Execute Inference with TensorFlow Inference Engine
    void Execute() override;

    /// @brief Release TensorFlow Inference Engine
    void Shutdown() override;

    /// @brief Set input data (waveform)
    ///
    /// @param waveform [in] - Waveform to be split
    /// @param nb_frames [in] - Number of frames within given Waveform
    /// @param nb_channels [in] - Number of channels within given Waveform
    void SetInputWaveform(const Waveform& waveform, const std::int32_t nb_frames,
                          const std::int32_t nb_channels) override;

    /// @brief Obtain Results for provided input waveform
    /// @return List of waveforms (split waveforms)
    Waveforms GetResults() const override;

    /// @brief Provide type of inference engine. Used to determine which inference engine it is.
    ///
    /// @return Inference Engine type (i.e. TensorFlow, TensorFlowLite, etc.)
    InferenceEngineType GetType() const override;

    /// @brief Provide configuration of the model selected
    ///
    /// @return configuration
    std::string GetConfiguration() const override;

  private:
    /// @brief Invokes Inference with TensorFlow APIs
    /// @return List of waveforms (split waveforms)
    virtual Waveforms InvokeInference() const;

    /// @brief Extracts model path from the provided command line arguments
    virtual std::string GetModelPath() const;

    /// @brief Model Configurations
    std::string configuration_;

    /// @brief Inference results (waveforms)
    Waveforms results_;

    /// @brief Saved Model bundle
    std::shared_ptr<tensorflow::SavedModelBundle> bundle_;

    /// @brief Input Tensor
    tensorflow::Tensor input_tensor_;

    /// @brief Output Tensors
    std::vector<tensorflow::Tensor> output_tensors_;

    /// @brief Output Tensors names
    std::vector<std::string> output_tensor_names_;

    /// @brief Model root directory
    const std::string model_dir_;
};

}  // namespace spleeter
#endif  /// SPLEETER_INFERENCE_ENGINE_TF_INFERENCE_ENGINE_H_
