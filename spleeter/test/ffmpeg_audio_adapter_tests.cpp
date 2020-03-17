///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/ffmpeg_audio_adapter.h"
#include "spleeter/i_audio_adapter.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace spleeter
{
namespace
{
class FfmpegAudioAdapterTest : public ::testing::Test
{
  public:
    FfmpegAudioAdapterTest() : unit_{std::make_unique<FfmpegAudioAdapter>()} {}

    ~FfmpegAudioAdapterTest() = default;

  protected:
    std::unique_ptr<IAudioAdapter> unit_;
};

}  // namespace
}  // namespace spleeter
