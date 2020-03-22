///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_INFERENCE_ENGINE_INFERENCE_ENGINE_H_
#define SPLEETER_INFERENCE_ENGINE_INFERENCE_ENGINE_H_

#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/inference_engine/i_inference_engine.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace spleeter
{
/// @brief Inference Engine Strategy
class InferenceEngine
{
  public:
    /// @brief Default Constructor
    InferenceEngine();

    /// @brief Constructor
    /// @param cli_options [in] - Command Line Interface Options
    explicit InferenceEngine(const CLIOptions& cli_options);

    /// @brief Destructor
    ~InferenceEngine() = default;

    /// @brief Initialise Inference Engine
    void Init();

    /// @brief Execute Inference with Inference Engine
    void Execute();

    /// @brief Release Inference Engine
    void Shutdown();

    /// @brief Select Inference Engine
    /// @param inference_engine [in] - Inference Engine type (TF, TFLite)
    void SelectInferenceEngine(const InferenceEngineType& inference_engine);

    /// @brief Set input data (waveform)
    /// @param waveform [in] - Waveform to be split
    void SetInputWaveform(const Waveform& waveform);

    /// @brief Obtain Results for provided input
    /// @return vector of pair of (confidence, label idx)
    Waveforms GetResults() const;

  private:
    /// @brief Command Line Options
    CLIOptions cli_options_;

    /// @brief Inference Engine
    std::unique_ptr<IInferenceEngine> inference_engine_;
};
}  // namespace spleeter
#endif  /// SPLEETER_INFERENCE_ENGINE_INFERENCE_ENGINE_H_
