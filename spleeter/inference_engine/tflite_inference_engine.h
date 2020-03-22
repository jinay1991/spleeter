///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_INFERENCE_ENGINE_TFLITE_INFERENCE_ENGINE_H_
#define SPLEETER_INFERENCE_ENGINE_TFLITE_INFERENCE_ENGINE_H_

#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/inference_engine/i_inference_engine.h"

#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace spleeter
{
/// @brief TFLite Inference Engine class
class TFLiteInferenceEngine : public IInferenceEngine
{
  public:
    /// @brief Default Constructor
    TFLiteInferenceEngine();

    /// @brief Constructor
    /// @param cli_options [in] - Command Line Interface Options
    explicit TFLiteInferenceEngine(const CLIOptions& cli_options);

    /// @brief Destructor
    ~TFLiteInferenceEngine() = default;

    /// @brief Initialise TFLite Inference Engine
    void Init() override;

    /// @brief Execute Inference with TFLite Inference Engine
    void Execute() override;

    /// @brief Release TFLite Inference Engine
    void Shutdown() override;

    /// @brief Set input data (waveform)
    /// @param waveform [in] - Waveform to be split
    void SetInputWaveform(const Waveform& waveform) override;

    /// @brief Obtain Results for provided input waveform
    /// @return List of waveforms (split waveforms)
    Waveforms GetResults() const override;

  private:
    /// @brief Invokes Inference with TFLite Interpreter
    /// @return List of waveforms (split waveforms)
    virtual Waveforms InvokeInference() const;

    /// @brief Extracts model path from the provided command line arguments
    virtual std::string GetModelPath() const;

    /// @brief Provide more detail logging
    virtual bool IsVerbosityEnabled() const;

    /// @brief Command Line Options
    CLIOptions cli_options_;

    /// @brief Inference results (waveforms)
    Waveforms results_;

    /// @brief TFLite Model Buffer Instance
    std::unique_ptr<tflite::FlatBufferModel> model_;

    /// @brief TFLite Model Interpreter instance
    std::unique_ptr<tflite::Interpreter> interpreter_;
};

}  // namespace spleeter
#endif  /// SPLEETER_INFERENCE_ENGINE_TFLITE_INFERENCE_ENGINE_H_
