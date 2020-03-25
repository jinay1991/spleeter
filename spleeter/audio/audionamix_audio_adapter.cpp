///
/// @file
/// @brief Contains definitions for FFMPEG Audio Adapter class methods
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/logging/logging.h"

namespace spleeter
{
AudionamixAudioAdapter::AudionamixAudioAdapter() {}

Waveform AudionamixAudioAdapter::Load(const std::string& path, const double /*offset*/, const double /*duration*/,
                                      const std::int32_t /*sample_rate*/)
{
    auto file = wave::File{};

    auto ret = file.Open(path, wave::OpenMode::kIn);
    ASSERT_CHECK(!ret) << "Unable to open " << path << "! (Returned: " << ret << ")";

    auto waveform = Waveform{};
    ret = file.Read(&waveform);
    ASSERT_CHECK(!ret) << "Unable to read " << path << "! (Returned: " << ret << ")";

    /// Save loaded audio properties
    audio_properties_.nb_frames = file.frame_number();
    audio_properties_.nb_channels = file.channel_number();
    audio_properties_.sample_rate = file.sample_rate();

    SPLEETER_LOG(DEBUG) << "Loaded waveform from " << path;
    return waveform;
}

void AudionamixAudioAdapter::Save(const std::string& path, const Waveform& data, const std::int32_t sample_rate,
                                  const std::string& /*codec*/, const std::int32_t /*bitrate*/)
{
    auto file = wave::File{};

    auto ret = file.Open(path, wave::OpenMode::kOut);
    ASSERT_CHECK(!ret) << "Unable to open " << path << "! (Returned: " << ret << ")";

    /// Set user provided configurations
    if (sample_rate != -1)
    {
        file.set_sample_rate(sample_rate);
    }
    file.set_channel_number(2);

    ret = file.Write(data);
    ASSERT_CHECK(!ret) << "Unable to write to " << path << "! (Returned: " << ret << ")";

    SPLEETER_LOG(DEBUG) << "Saved waveform to " << path;
}

AudioProperties AudionamixAudioAdapter::GetProperties() const { return audio_properties_; }

}  // namespace spleeter
