///
/// @file
/// @copyright Copyright (c) 2020. MIT License.
///
#ifndef SPLEETER_INFERENCE_ENGINE_TORCH_INFERENCE_ENGINE_H_
#define SPLEETER_INFERENCE_ENGINE_TORCH_INFERENCE_ENGINE_H_

#include "spleeter/inference_engine/i_inference_engine.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace spleeter
{
class TorchInferenceEngine : public IInferenceEngine
{
  public:
    /// @brief Default Constructor
    TorchInferenceEngine();

    /// @brief Constructor
    ///
    /// @param configuration [in] - configurations
    explicit TorchInferenceEngine(const std::string& configuration);

    /// @brief Destructor
    ~TorchInferenceEngine() = default;

    /// @brief Initialise Inference Engine
    void Init() override;

    /// @brief Execute Inference with Inference Engine
    void Execute() override;

    /// @brief Release Inference Engine
    void Shutdown() override;

    /// @brief Set input data (waveform)
    /// @param waveform [in] - Waveform to be split
    /// @param nb_frames [in] - Number of frames within given Waveform
    /// @param nb_channels [in] - Number of channels within given Waveform
    void SetInputWaveform(const Waveform& waveform,
                          const std::int32_t nb_frames,
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
    /// @brief Model Configurations
    std::string configuration_;
};
}  // namespace spleeter

#endif  /// SPLEETER_INFERENCE_ENGINE_TORCH_INFERENCE_ENGINE_H_