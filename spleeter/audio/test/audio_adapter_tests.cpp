///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/audio/ffmpeg_audio_adapter.h"
#include "spleeter/logging/logging.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <string>

namespace spleeter
{
namespace
{
template <typename T>
class AudioAdapterTest : public ::testing::Test
{
  public:
    AudioAdapterTest() : test_sample_rate_{44100}, cli_options_{}, test_waveform_{}, test_waveform_properties_{} {}
    ~AudioAdapterTest() = default;

  protected:
    void SetUp() override
    {
        auto test_input = "external/audio_example/file/audio_example.wav";
        auto audio_adapter = AudionamixAudioAdapter{};
        test_waveform_ = audio_adapter.Load(test_input, 0, -1, 44100);
        test_waveform_properties_ = audio_adapter.GetProperties();
    }

    Waveform GetTestWaveform() const { return test_waveform_; }

    AudioProperties GetTestWaveformProperties() const { return test_waveform_properties_; }

    const std::int32_t test_sample_rate_;
    CLIOptions cli_options_;
    Waveform test_waveform_;
    AudioProperties test_waveform_properties_;

    T unit_;
};
TYPED_TEST_SUITE_P(AudioAdapterTest);

/// @test In this, test that Load method returns raw waveform and updates audio properties
TYPED_TEST_P(AudioAdapterTest, GivenAudioFile_ExpectRawWaveform)
{
    auto actual = this->unit_.Load(this->cli_options_.inputs, this->cli_options_.offset, this->cli_options_.duration,
                                   this->test_sample_rate_);

    EXPECT_FALSE(actual.empty());
    EXPECT_EQ(959664U, actual.size());

    const auto actual_properties = this->unit_.GetProperties();
    const auto expected_properties = this->GetTestWaveformProperties();
    EXPECT_EQ(expected_properties.nb_channels, actual_properties.nb_channels);
    EXPECT_EQ(expected_properties.nb_frames, actual_properties.nb_frames);
    EXPECT_EQ(expected_properties.sample_rate, actual_properties.sample_rate);
}

/// @test In this, test that Save method saves the file and validated against source audio properties.
TYPED_TEST_P(AudioAdapterTest, GivenRawWaveform_ExpectSavedFileHasSameProperties)
{
    auto bitrate = std::int32_t{192000};
    auto test_codec = "wav";
    auto test_results = "test_sample.wav";
    this->unit_.Save(test_results, this->GetTestWaveform(), this->test_sample_rate_, test_codec, bitrate);

    auto actual =
        this->unit_.Load(test_results, this->cli_options_.offset, this->cli_options_.duration, this->test_sample_rate_);
    EXPECT_FALSE(actual.empty());
    EXPECT_EQ(959664U, actual.size());
    const auto actual_properties = this->unit_.GetProperties();
    const auto expected_properties = this->GetTestWaveformProperties();
    EXPECT_EQ(expected_properties.nb_channels, actual_properties.nb_channels);
    EXPECT_EQ(expected_properties.nb_frames, actual_properties.nb_frames);
    EXPECT_EQ(expected_properties.sample_rate, actual_properties.sample_rate);
}

REGISTER_TYPED_TEST_SUITE_P(AudioAdapterTest, GivenAudioFile_ExpectRawWaveform,
                            GivenRawWaveform_ExpectSavedFileHasSameProperties);

typedef ::testing::Types<AudionamixAudioAdapter> AudioAdapterTestTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(TypeTests, AudioAdapterTest, AudioAdapterTestTypes);

}  // namespace
}  // namespace spleeter
