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
        cli_options_.output_path = "/tmp/separated_audio2";
        const auto stem{std::to_string(GetParam()) + "stems"};
        cli_options_.configuration = "spleeter:" + stem;
        cli_options_.inference_engine_params.model_path = "external/models/" + stem + "/" + stem + ".tflite";
        cli_options_.inference_engine_params.output_tensor_names = GetOutputTensorNames(cli_options_.configuration);
        cli_options_.inference_engine_params.configuration = cli_options_.configuration;

        test_waveform_ = audio_adapter_.Load(cli_options_.inputs, 0, -1, 44100);

        unit_ = std::make_unique<Separator>(cli_options_.inference_engine_params, cli_options_.mwf);
    }

    static std::vector<std::string> GetOutputTensorNames(const std::string& configuration)
    {
        auto output_tensor_names = std::vector<std::string>{};
        if (configuration == "spleeter:2stems")
        {
            output_tensor_names = std::vector<std::string>{"strided_slice_13", "strided_slice_23"};
        }

        else if (configuration == "spleeter:4stems")
        {
            output_tensor_names = std::vector<std::string>{
                "strided_slice_13", "strided_slice_23", "strided_slice_33", "strided_slice_43"};
        }
        else  // default to "spleeter:5stems"
        {
            output_tensor_names = std::vector<std::string>{
                "strided_slice_18", "strided_slice_38", "strided_slice_48", "strided_slice_28", "strided_slice_58"};
        }
        return output_tensor_names;
    }

    CLIOptions cli_options_;
    std::unique_ptr<Separator> unit_;
    Waveform test_waveform_;
    AudionamixAudioAdapter audio_adapter_;
};

TEST_P(SeparatorTest, GivenConfiguration_ExpectSeparatedWaveforms)
{
    auto actual = unit_->Separate(test_waveform_);
    EXPECT_EQ(GetParam(), actual.size());
}

/// @test In this, test that SeparateToFile saves all the resultant 5 waveforms to file (for given 5stems model)
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

INSTANTIATE_TEST_CASE_P(Configurations, SeparatorTest, ::testing::Values(2, 4, 5));

}  // namespace
}  // namespace spleeter
