///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_DATA_TYPES_AUDIO_PROPERTIES_H_
#define SPLEETER_DATA_TYPES_AUDIO_PROPERTIES_H_

#include <cstdint>

namespace spleeter
{
struct AudioProperties
{
    std::uint64_t nb_frames;
    std::uint64_t nb_channels;
    std::uint32_t sample_rate;
};
}  // namespace spleeter
#endif  /// SPLEETER_DATA_TYPES_AUDIO_PROPERTIES_H_
