///
/// @file
/// @brief Contains Inference Interface Engine definition
/// @copyright Copyright (c) 2020. MIT License
///
#ifndef SPLEETER_INFERENCE_ENGINE_I_INFERENCE_ENGINE_H
#define SPLEETER_INFERENCE_ENGINE_I_INFERENCE_ENGINE_H

#include "spleeter/datatypes/waveform.h"

#include <memory>

namespace spleeter
{

/// @brief Inference Engine Interface class
class IInferenceEngine
{
  public:
    /// @brief Destructor
    virtual ~IInferenceEngine() = default;

    /// @brief Initialise Inference Engine
    virtual void Init() = 0;

    /// @brief Execute Inference with Inference Engine
    /// @param waveform [in] - Waveform to be split
    virtual void Execute(const Waveform& waveform) = 0;

    /// @brief Release Inference Engine
    virtual void Shutdown() = 0;

    /// @brief Obtain Results for provided input waveform
    /// @return List of waveforms (split waveforms)
    virtual Waveforms GetResults() const = 0;
};

/// @brief InferenceEngine unique instance pointer
using InferenceEnginePtr = std::unique_ptr<IInferenceEngine>;

}  // namespace spleeter
#endif  /// SPLEETER_INFERENCE_ENGINE_I_INFERENCE_ENGINE_H
