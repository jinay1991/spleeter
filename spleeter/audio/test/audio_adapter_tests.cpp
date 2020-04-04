///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include <algorithm>
#include <cstdint>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/audio/ffmpeg_audio_adapter.h"

namespace spleeter
{
namespace
{
template <typename T>
class AudioAdapterTest : public ::testing::Test
{
  public:
    AudioAdapterTest()
        : test_sample_rate_{44100},
          test_input_{"external/audio_example/file/audio_example.wav"},
          test_waveform_{},
          test_waveform_properties_{}
    {
    }

    ~AudioAdapterTest() = default;

    Waveform GetTestWaveform() const { return test_waveform_; }

    AudioProperties GetTestWaveformProperties() const { return test_waveform_properties_; }

  protected:
    void SetUp() override
    {
        AudionamixAudioAdapter audio_adapter{};
        test_waveform_ = audio_adapter.Load(test_input_, 0, -1, test_sample_rate_);
        test_waveform_properties_ = audio_adapter.GetProperties();

        unit_ = std::make_unique<T>();
    }

    const std::int32_t test_sample_rate_;
    const std::string test_input_;
    Waveform test_waveform_;
    AudioProperties test_waveform_properties_;

    std::unique_ptr<IAudioAdapter> unit_;
};
TYPED_TEST_SUITE_P(AudioAdapterTest);

/// @test In this, test that Load method returns raw waveform and updates audio properties
TYPED_TEST_P(AudioAdapterTest, DISABLED_GivenAudioFile_ExpectRawWaveform)
{
    const auto actual = this->unit_->Load(this->test_input_, 0.0, -1, this->test_sample_rate_);
    EXPECT_FALSE(actual.empty());
    EXPECT_EQ(959664U, actual.size());
    EXPECT_THAT(this->test_waveform_, ::testing::ContainerEq(actual));

    const auto actual_properties = this->unit_->GetProperties();
    const auto expected_properties = this->GetTestWaveformProperties();
    EXPECT_EQ(expected_properties.nb_channels, actual_properties.nb_channels);
    EXPECT_EQ(expected_properties.nb_frames, actual_properties.nb_frames);
    EXPECT_EQ(expected_properties.sample_rate, actual_properties.sample_rate);
}

/// @test In this, test that Save method saves the file and validated against source audio properties.
TYPED_TEST_P(AudioAdapterTest, GivenRawWaveform_ExpectSavedFileHasSameProperties)
{
    const auto bitrate = std::int32_t{192000};
    const auto test_codec = "wav";
    const auto test_results = "test_sample.wav";

    this->unit_->Save(test_results, this->GetTestWaveform(), this->test_sample_rate_, test_codec, bitrate);

    AudionamixAudioAdapter audio_adapter{};
    const auto actual = audio_adapter.Load(test_results, 0, -1, this->test_sample_rate_);
    EXPECT_FALSE(actual.empty());
    EXPECT_EQ(959664U, actual.size());
    // EXPECT_THAT(this->test_waveform_, ::testing::ContainerEq(actual));

    const auto actual_properties = audio_adapter.GetProperties();
    const auto expected_properties = this->GetTestWaveformProperties();
    EXPECT_EQ(expected_properties.nb_channels, actual_properties.nb_channels);
    EXPECT_EQ(expected_properties.nb_frames, actual_properties.nb_frames);
    EXPECT_EQ(expected_properties.sample_rate, actual_properties.sample_rate);
}

REGISTER_TYPED_TEST_SUITE_P(AudioAdapterTest,
                            DISABLED_GivenAudioFile_ExpectRawWaveform,
                            GivenRawWaveform_ExpectSavedFileHasSameProperties);

typedef ::testing::Types<FfmpegAudioAdapter> AudioAdapterTestTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(TypeTests, AudioAdapterTest, AudioAdapterTestTypes);

}  // namespace
}  // namespace spleeter
