///
/// @file
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/inference_engine/tf_inference_engine.h"
#include "spleeter/logging/logging.h"

#include <sys/time.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace spleeter
{
TFInferenceEngine::TFInferenceEngine() : TFInferenceEngine{CLIOptions{}} {}

TFInferenceEngine::TFInferenceEngine(const CLIOptions& cli_options) : cli_options_{cli_options} {}

void TFInferenceEngine::Init() {}

void TFInferenceEngine::Execute()
{
    SetInputWaveform(Waveform{});
    results_ = InvokeInference();
}

void TFInferenceEngine::Shutdown() {}

Waveforms TFInferenceEngine::InvokeInference() const { return Waveforms{}; }

void TFInferenceEngine::SetInputWaveform(const Waveform& waveform) {}

Waveforms TFInferenceEngine::GetResults() const { return results_; }

std::string TFInferenceEngine::GetModelPath() const { return "data/model.t"; }

bool TFInferenceEngine::IsVerbosityEnabled() const { return cli_options_.verbose; }

}  // namespace spleeter
