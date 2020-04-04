///
/// @file
/// @brief Contains component tests for Spleeter
/// @copyright Copyright (c) 2020, MIT License
///
#include <memory>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "spleeter/spleeter.h"

namespace spleeter
{
namespace
{
TEST(SpleeterSpec, ScenarioFiveStems_GivenAudioWaveform_ExpectFiveSplitAudioWaveforms)
{
    auto cli_options = CLIOptions{};
    Spleeter unit{cli_options};
    unit.Init();

    unit.Execute();
}

}  // namespace
}  // namespace spleeter
