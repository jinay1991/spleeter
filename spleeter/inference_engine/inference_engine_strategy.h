///
/// @file
/// @copyright Copyright (c) 2020. MIT License
///
#ifndef SPLEETER_INFERENCE_ENGINE_INFERENCE_ENGINE_STRATEGY_H
#define SPLEETER_INFERENCE_ENGINE_INFERENCE_ENGINE_STRATEGY_H

#include "spleeter/datatypes/inference_engine.h"
#include "spleeter/inference_engine/i_inference_engine.h"

#include <memory>

namespace spleeter
{

/// @brief Inference Engine Strategy
class InferenceEngineStrategy final
{
  public:
    /// @brief Default Constructor
    InferenceEngineStrategy();

    /// @brief Initialise Inference Engine
    void Init();

    /// @brief Execute Inference with Inference Engine
    /// @param waveform [in] - Waveform to be split
    void Execute(const Waveform& waveform);

    /// @brief Release Inference Engine
    void Shutdown();

    /// @brief Select Inference Engine
    /// @param inference_engine_type [in] Inference Engine type (TF, TFLite)
    /// @param inference_engine_param [in] Inference Engine Parameters
    void SelectInferenceEngine(const InferenceEngineType& inference_engine_type,
                               const InferenceEngineParameters& inference_engine_parameters);

    /// @brief Obtain Results for provided input waveform
    /// @return List of waveforms (split waveforms)
    Waveforms GetResults() const;

    /// @brief Provide selected inference engine type
    /// @return InferenceEngineType
    InferenceEngineType GetInferenceEngineType() const;

  private:
    /// @brief Inference Engine
    std::unique_ptr<IInferenceEngine> inference_engine_;

    /// @brief Inference Engine Type
    InferenceEngineType inference_engine_type_;
};
}  // namespace spleeter
#endif  /// SPLEETER_INFERENCE_ENGINE_INFERENCE_ENGINE_STRATEGY_H
