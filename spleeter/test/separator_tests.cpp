///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/i_separator.h"
#include "spleeter/separator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <experimental/filesystem>

namespace spleeter
{
namespace
{
class SeparatorTest : public ::testing::TestWithParam<std::int32_t>
{
  public:
    SeparatorTest() : cli_options_{}, test_waveform_{}, audio_adapter_{} {}

  protected:
    void SetUp() override
    {
        cli_options_.configuration = "spleeter:" + std::to_string(GetParam()) + "stems";

        test_waveform_ = audio_adapter_.Load(cli_options_.inputs, 0, -1, 44100);

        unit_ = std::make_unique<Separator>(cli_options_.configuration, cli_options_.mwf);
    }

    CLIOptions cli_options_;
    std::unique_ptr<Separator> unit_;
    Waveform test_waveform_;
    AudionamixAudioAdapter audio_adapter_;
};

TEST_P(SeparatorTest, GivenConfiguration_ExpectSeparatedWaveforms)
{
    auto actual = unit_->Separate(test_waveform_, audio_adapter_.GetProperties());
    EXPECT_EQ(GetParam(), actual.size());
}

/// @brief In this, test that SeparateToFile saves all the resultant 5 waveforms to file (for given 5stems model)
TEST_P(SeparatorTest, GivenTypicalInputs_ExpectSeparatedToFiles)
{
    unit_->SeparateToFile(cli_options_.inputs,
                          cli_options_.output_path,
                          cli_options_.audio_adapter,
                          cli_options_.offset,
                          cli_options_.duration,
                          cli_options_.codec,
                          cli_options_.bitrate,
                          cli_options_.filename_format,
                          false);

    EXPECT_TRUE(std::experimental::filesystem::exists(cli_options_.output_path));
}

INSTANTIATE_TEST_CASE_P(Configurations, SeparatorTest, ::testing::Values(4, 5));

}  // namespace
}  // namespace spleeter
