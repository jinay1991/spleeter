///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/inference_engine/tf_inference_engine.h"
#include "spleeter/logging/logging.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <wave/file.h>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace spleeter
{
namespace
{
class TFInferenceEngineTest : public ::testing::TestWithParam<std::int32_t>
{
  protected:
    void SetUp() override
    {
        std::string test_input = "external/audio_example/file/audio_example.wav";
        std::string configuration = "spleeter:" + std::to_string(GetParam()) + "stems";

        auto audio_adapter = AudionamixAudioAdapter{};
        const auto test_waveform = audio_adapter.Load(test_input, 0, -1, 44100);
        const auto test_waveform_properties = audio_adapter.GetProperties();

        unit_ = std::make_unique<TFInferenceEngine>(configuration);
        unit_->Init();

        unit_->SetInputWaveform(
            test_waveform, test_waveform_properties.nb_frames, test_waveform_properties.nb_channels);
    }

    void TearDown() override { unit_->Shutdown(); }

    std::unique_ptr<TFInferenceEngine> unit_;
};

TEST_P(TFInferenceEngineTest, GivenConfigurations_ExpectSeparatedWaveforms)
{
    unit_->Execute();

    auto actual = unit_->GetResults();
    EXPECT_EQ(GetParam(), actual.size());
}
INSTANTIATE_TEST_CASE_P(Configuration, TFInferenceEngineTest, ::testing::Values(2, 4, 5));

}  // namespace
}  // namespace spleeter
