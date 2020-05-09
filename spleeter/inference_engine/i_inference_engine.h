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
    kTorch = 2U
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
    /// @param nb_frames [in] - Number of frames within given Waveform
    /// @param nb_channels [in] - Number of channels within given Waveform
    virtual void SetInputWaveform(const Waveform& waveform,
                                  const std::int32_t nb_frames,
                                  const std::int32_t nb_channels) = 0;

    /// @brief Obtain Results for provided input waveform
    /// @return List of waveforms (split waveforms)
    virtual Waveforms GetResults() const = 0;

    /// @brief Provide type of inference engine. Used to determine which inference engine it is.
    ///
    /// @return Inference Engine type (i.e. TensorFlow, TensorFlowLite, etc.)
    virtual InferenceEngineType GetType() const = 0;

    /// @brief Provide configuration of the model selected
    ///
    /// @return configuration
    virtual std::string GetConfiguration() const = 0;
};
inline const char* to_string(const InferenceEngineType& inference_engine_type)
{
    switch (inference_engine_type)
    {
        case InferenceEngineType::kTensorFlow:
            return "kTensorFlow";
        case InferenceEngineType::kTensorFlowLite:
            return "kTensorFlowLite";
        case InferenceEngineType::kTorch:
            return "kTorch";
        default:
            return "ERROR: Invalid type provided";
    }
    return "ERROR: Invalid type provided";
}

inline std::ostream& operator<<(std::ostream& stream, const InferenceEngineType& inference_engine_type)
{
    const char* name = to_string(inference_engine_type);
    stream << name;
    return stream;
}

}  // namespace spleeter
#endif  /// SPLEETER_I_INFERENCE_ENGINE_H_
