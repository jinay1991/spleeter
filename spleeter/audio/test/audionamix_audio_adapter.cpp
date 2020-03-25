///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/argument_parser/cli_options.h"
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
class AudionamixAudioAdapterTest : public ::testing::Test
{
  public:
    AudionamixAudioAdapterTest() : test_sample_rate_{44100}, cli_options_{}, unit_{} {}

    ~AudionamixAudioAdapterTest() = default;

  protected:
    Waveform GetTestWaveform() const
    {
        return AudionamixAudioAdapter().Load(cli_options_.inputs, cli_options_.offset, cli_options_.duration,
                                             test_sample_rate_);
    }

    AudioProperties GetTestWaveformProperties() const
    {
        auto audio_properties = AudioProperties{};

        audio_properties.sample_rate = test_sample_rate_;
        audio_properties.nb_frames = 479832U;
        audio_properties.nb_channels = 2U;

        return audio_properties;
    }

    const std::int32_t test_sample_rate_;
    CLIOptions cli_options_;
    AudionamixAudioAdapter unit_;
};

/// @test In this, test that Load method returns raw waveform and updates audio properties
TEST_F(AudionamixAudioAdapterTest, GivenAudioFile_ExpectRawWaveform)
{
    auto actual = unit_.Load(cli_options_.inputs, cli_options_.offset, cli_options_.duration, test_sample_rate_);

    EXPECT_FALSE(actual.empty());
    EXPECT_EQ(959664U, actual.size());

    const auto actual_properties = unit_.GetProperties();
    const auto expected_properties = GetTestWaveformProperties();
    EXPECT_EQ(expected_properties.nb_channels, actual_properties.nb_channels);
    EXPECT_EQ(expected_properties.nb_frames, actual_properties.nb_frames);
    EXPECT_EQ(expected_properties.sample_rate, actual_properties.sample_rate);
}

/// @test In this, test that Save method saves the file and validated against source audio properties.
TEST_F(AudionamixAudioAdapterTest, GivenRawWaveform_ExpectSavedFileHasSameProperties)
{
    auto bitrate = std::int32_t{192000};
    auto test_codec = "wav";
    auto test_results = "test_sample.wav";
    unit_.Save(test_results, GetTestWaveform(), test_sample_rate_, test_codec, bitrate);

    auto actual = unit_.Load(test_results, cli_options_.offset, cli_options_.duration, test_sample_rate_);
    EXPECT_FALSE(actual.empty());
    EXPECT_EQ(959664U, actual.size());
    const auto actual_properties = unit_.GetProperties();
    const auto expected_properties = GetTestWaveformProperties();
    EXPECT_EQ(expected_properties.nb_channels, actual_properties.nb_channels);
    EXPECT_EQ(expected_properties.nb_frames, actual_properties.nb_frames);
    EXPECT_EQ(expected_properties.sample_rate, actual_properties.sample_rate);
}
}  // namespace
}  // namespace spleeter
