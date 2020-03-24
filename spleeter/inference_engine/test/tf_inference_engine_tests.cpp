///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/inference_engine/tf_inference_engine.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace spleeter
{
namespace
{
TEST(TFInferenceEngineTest, DefaultConstructor)
{
    auto unit = TFInferenceEngine{};
    unit.Init();
    unit.SetInputWaveform(Waveform{});

    unit.Execute();

    auto actual = unit.GetResults();
    EXPECT_THAT(5U, actual.size());
}

TEST(TFInferenceEngineTest, GivenDefaultConfiguration_ExpectTwoResultantWaveforms)
{
    auto cli_options = CLIOptions{};
    cli_options.configuration = "spleeter:4stems";
    auto unit = TFInferenceEngine{cli_options};
    unit.Init();
    unit.SetInputWaveform(Waveform{});

    unit.Execute();
    auto actual = unit.GetResults();
    EXPECT_THAT(4U, actual.size());
}

}  // namespace
}  // namespace spleeter
