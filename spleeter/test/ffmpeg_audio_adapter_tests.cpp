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
class FfmpegAudioAdapterTest : public ::testing::Test
{
  public:
    FfmpegAudioAdapterTest()
        : unit_{std::make_unique<FfmpegAudioAdapter>()},
          test_audio_description_{"audio_example.mp3"},
          test_offset_{0.0},
          test_duration_{600.0},
          test_sample_rate_{44100}
    {
    }

    ~FfmpegAudioAdapterTest() = default;

  protected:
    std::unique_ptr<IAudioAdapter> unit_;

    const std::string test_audio_description_;
    const double test_offset_;
    const double test_duration_;
    const std::int32_t test_sample_rate_;
};

TEST_F(FfmpegAudioAdapterTest, DISABLED_Load)
{
    // auto waveform_sample_rate_pair =
    //     unit_->Load(test_audio_description_, test_offset_, test_duration_, test_sample_rate_);
    // auto waveform = waveform_sample_rate_pair.first;
    // auto sample_rate = waveform_sample_rate_pair.second;

    // ASSERT_EQ(sample_rate, test_sample_rate_);
    // ASSERT_FALSE(waveform.empty());
    // ASSERT_EQ(waveform.size(), 2);
    // ASSERT_EQ(waveform[0].size(), 479832);
    // ASSERT_EQ(waveform[1].size(), 2);
}

TEST_F(FfmpegAudioAdapterTest, DISABLED_LoadError)
{
    // EXPECT_THROW(unit_->Load("Paris City Jazz", test_offset_, test_duration_, test_sample_rate_),
    // std::runtime_error);
}

TEST_F(FfmpegAudioAdapterTest, DISABLED_Save)
{
    // auto audio_data = Waveform{};
    // auto path = "/tmp/ffmpeg-save.mp3";
    // unit_->Save(path, audio_data, test_sample_rate_, "mp3", "128k");
    // probe = ffmpeg.probe(TEST_AUDIO_DESCRIPTOR)
    // assert len(probe['streams']) == 1
    // stream = probe['streams'][0]
    // assert stream['codec_type'] == 'audio'
    // assert stream['channels'] == 2
    // assert stream['duration'] == '10.919184'
}

}  // namespace
}  // namespace spleeter
