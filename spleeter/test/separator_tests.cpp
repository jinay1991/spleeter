///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/i_separator.h"
#include "spleeter/separator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace spleeter
{
namespace
{
class SeparatorTest : public ::testing::Test
{
  public:
    SeparatorTest() : cli_options_{}, unit_{cli_options_.configuration, cli_options_.mwf} {}

  protected:
    CLIOptions cli_options_;

    Separator unit_;
};
/// @brief In this, test that Separate is separating given waveform in to 5 waveforms (for given 5stems model)
TEST_F(SeparatorTest, GivenWaveform_ExpectSeparatedWaveforms)
{
    auto actual = unit_.Separate(Waveform{});
    EXPECT_EQ(5U, actual.size());
}

/// @brief In this, test that SeparateToFile saves all the resultant 5 waveforms to file (for given 5stems model)
TEST_F(SeparatorTest, GivenWaveform_ExpectSeparatedToFiles)
{
    unit_.SeparateToFile(cli_options_.inputs, cli_options_.output_path, cli_options_.audio_adapter, cli_options_.offset,
                         cli_options_.duration, cli_options_.codec, cli_options_.bitrate, cli_options_.filename_format,
                         false);
}
}  // namespace
}  // namespace spleeter
