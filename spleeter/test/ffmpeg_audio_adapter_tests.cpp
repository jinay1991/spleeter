///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/ffmpeg_audio_adapter.h"
#include "spleeter/i_audio_adapter.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

namespace spleeter
{
namespace
{
class AudioAdapterTest : public ::testing::Test
{
  public:
    AudioAdapterTest()
        : audio_adapter_{std::make_unique<FfmpegAudioAdapter>()},
          test_audio_description_{"data/audio_example.mp3"},
          test_offset_{0.0},
          test_duration_{600.0},
          test_sample_rate_{44100},
          test_codec_{"mp3"},
          test_bitrate_{"128k"}
    {
    }

    ~AudioAdapterTest() = default;

  protected:
    std::unique_ptr<IAudioAdapter> audio_adapter_;

    const std::string test_audio_description_;
    const double test_offset_;
    const double test_duration_;
    const std::int32_t test_sample_rate_;
    const std::string test_codec_;
    const std::string test_bitrate_;
};

TEST_F(AudioAdapterTest, Load)
{
    auto waveform_sample_rate_pair =
        audio_adapter_->Load(test_audio_description_, test_offset_, test_duration_, test_sample_rate_);
    // auto waveform = waveform_sample_rate_pair.first;
    // auto sample_rate = waveform_sample_rate_pair.second;

    // ASSERT_EQ(sample_rate, test_sample_rate_);
    // ASSERT_FALSE(waveform.empty());
    // ASSERT_EQ(waveform.size(), 2);
    // ASSERT_EQ(waveform[0].size(), 479832);
    // ASSERT_EQ(waveform[1].size(), 2);
}

TEST_F(AudioAdapterTest, DISABLED_LoadError)
{
    // EXPECT_THROW(audio_adapter_->Load("Paris City Jazz", test_offset_, test_duration_, test_sample_rate_),
    // std::runtime_error);
}

TEST_F(AudioAdapterTest, Save)
{
    auto audio_data = Waveform{};
    auto path = "/tmp/ffmpeg-save.wav";
    auto codec = "wav";
    auto bitrate = 128000;
    auto sample_rate = 44100;

    audio_adapter_->Save(path, audio_data, sample_rate, codec, bitrate);
}

}  // namespace
}  // namespace spleeter
