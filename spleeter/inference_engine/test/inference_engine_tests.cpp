///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/inference_engine/inference_engine.h"

namespace spleeter
{
namespace
{
class InferenceEngineTest : public ::testing::Test
{
  public:
    InferenceEngineTest() : unit_{}, test_waveform_{}, test_waveform_properties_{} {}

  protected:
    void SetUp() override
    {
        std::string test_input = "external/audio_example/file/audio_example.wav";
        std::string configuration = "spleeter:5stems";

        auto audio_adapter = AudionamixAudioAdapter{};
        test_waveform_ = audio_adapter.Load(test_input, 0, -1, 44100);
        test_waveform_properties_ = audio_adapter.GetProperties();

        unit_.SelectInferenceEngine(InferenceEngineType::kTensorFlow, configuration);
        unit_.Init();
        unit_.SetInputWaveform(
            test_waveform_, test_waveform_properties_.nb_frames, test_waveform_properties_.nb_channels);
    }

    void TearDown() override { unit_.Shutdown(); }

    InferenceEngine unit_;
    Waveform test_waveform_;
    AudioProperties test_waveform_properties_;
};

TEST_F(InferenceEngineTest, SelectInferenceEngine)
{
    unit_.SelectInferenceEngine(InferenceEngineType::kTensorFlow, "spleeter:2stems");
    EXPECT_EQ(InferenceEngineType::kTensorFlow, unit_.GetType());
    EXPECT_EQ("spleeter:2stems", unit_.GetConfiguration());

    unit_.SelectInferenceEngine(InferenceEngineType::kTensorFlowLite, "spleeter:2stems");
    EXPECT_EQ(InferenceEngineType::kTensorFlowLite, unit_.GetType());
    EXPECT_EQ("spleeter:2stems", unit_.GetConfiguration());
}

TEST_F(InferenceEngineTest, SanityForTensorFlowInferenceEngine)
{
    unit_.SelectInferenceEngine(InferenceEngineType::kTensorFlow, "spleeter:5stems");
    unit_.Init();
    unit_.SetInputWaveform(test_waveform_, test_waveform_properties_.nb_frames, test_waveform_properties_.nb_channels);

    unit_.Execute();

    auto actual = unit_.GetResults();
    EXPECT_EQ(InferenceEngineType::kTensorFlow, unit_.GetType());
    EXPECT_EQ(5U, actual.size());
}

TEST_F(InferenceEngineTest, DISABLED_SanityForTensorFlowLiteInferenceEngine)
{
    unit_.SelectInferenceEngine(InferenceEngineType::kTensorFlowLite, "spleeter:5stems");
    unit_.Init();
    unit_.SetInputWaveform(test_waveform_, test_waveform_properties_.nb_frames, test_waveform_properties_.nb_channels);
    unit_.Execute();

    auto actual = unit_.GetResults();
    EXPECT_EQ(InferenceEngineType::kTensorFlowLite, unit_.GetType());
    EXPECT_EQ(5U, actual.size());
}
}  // namespace
}  // namespace spleeter
