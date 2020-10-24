///
/// @file
/// @copyright Copyright (c) 2020. MIT License
///
#ifndef SPLEETER_INFERENCE_ENGINE_NULL_INFERENCE_ENGINE_H
#define SPLEETER_INFERENCE_ENGINE_NULL_INFERENCE_ENGINE_H

#include "spleeter/datatypes/inference_engine.h"
#include "spleeter/inference_engine/i_inference_engine.h"

namespace spleeter
{
/// @brief Null Inference Engine class
class NullInferenceEngine final : public IInferenceEngine
{
  public:
    /// @brief Constructor
    /// @param params[in] Inference Engine Parameters such as model input/output node names
    explicit NullInferenceEngine(const InferenceEngineParameters& params);

    /// @brief Initialise Null Inference Engine
    void Init() override;

    /// @brief Execute Inference with Null Inference Engine
    /// @param waveform [in] - Waveform to be split
    void Execute(const Waveform& waveform) override;

    /// @brief Release Null Inference Engine
    void Shutdown() override;

    /// @brief Provide results in terms of Matrix
    /// @return List of waveforms (split waveforms)
    Waveforms GetResults() const override;

  private:
    /// @brief Output Tensors saved as Waveforms
    const Waveforms results_;
};

}  // namespace spleeter

#endif  /// SPLEETER_INFERENCE_ENGINE_NULL_INFERENCE_ENGINE_H
