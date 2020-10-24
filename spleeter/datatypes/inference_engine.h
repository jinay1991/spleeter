///
/// @file
/// @copyright Copyright (c) 2020. MIT License
///
#ifndef SPLEETER_DATATYPES_INFERENCE_ENGINE_H
#define SPLEETER_DATATYPES_INFERENCE_ENGINE_H

#include <ostream>
#include <string>
#include <vector>

namespace spleeter
{
/// @brief List of Inference Engine supported
enum class InferenceEngineType : std::uint8_t
{
    kTensorFlowLite = 0U,
    kTensorFlow = 1U,
    kInvalid = 255U
};

/// @brief InferenceEngine Parameters
struct InferenceEngineParameters
{
    /// @brief Path to Model
    std::string model_path{};

    /// @brief Input Node/Tensor Name
    std::string input_tensor_name{};

    /// @brief List of Output Nodes/Tensors Name
    std::vector<std::string> output_tensor_names{};

    /// @brief Path to Model configurations
    std::string configuration{};
};

inline const char* to_string(const InferenceEngineType& inference_engine_type)
{
    switch (inference_engine_type)
    {
        case InferenceEngineType::kTensorFlow:
            return "kTensorFlow";
        case InferenceEngineType::kTensorFlowLite:
            return "kTensorFlowLite";
        default:
            return "ERROR: Unknown InferenceEngineType.";
    }
    return "ERROR: Unknown InferenceEngineType.";
}

inline std::ostream& operator<<(std::ostream& stream, const InferenceEngineType& inference_engine_type)
{
    const char* name = to_string(inference_engine_type);
    stream << name;
    return stream;
}

}  // namespace spleeter
#endif  /// SPLEETER_DATATYPES_INFERENCE_ENGINE_H
