///
/// @file
/// @brief Contains Inference Interface Engine definition
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_I_INFERENCE_ENGINE_H_
#define SPLEETER_I_INFERENCE_ENGINE_H_

#include "spleeter/data_types/waveform.h"

#include <cstdint>
#include <string>
#include <vector>

namespace spleeter
{
/// @brief List of Inference Engine supported
enum class InferenceEngineType : std::uint32_t
{
    kTensorFlowLite = 0U,
    kTensorFlow = 1U,
    // kTorch = 2, /// not-supported
    // kCaffe = 3, /// not-supported
};
/// @brief Inference Engine Interface class
class IInferenceEngine
{
  public:
    /// @brief Destructor
    virtual ~IInferenceEngine() = default;

    /// @brief Initialise Inference Engine
    virtual void Init() = 0;

    /// @brief Execute Inference with Inference Engine
    virtual void Execute() = 0;

    /// @brief Release Inference Engine
    virtual void Shutdown() = 0;

    /// @brief Set input data (waveform)
    /// @param waveform [in] - Waveform to be split
    virtual void SetInputWaveform(const Waveform& waveform) = 0;

    /// @brief Obtain Results for provided input waveform
    /// @return List of waveforms (split waveforms)
    virtual Waveforms GetResults() const = 0;
};
}  // namespace spleeter
#endif  /// SPLEETER_I_INFERENCE_ENGINE_H_
