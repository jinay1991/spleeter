///
/// @file
/// @brief Contains Data Structure to hold Waveform
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_DATA_TYPES_WAVEFORM_H_
#define SPLEETER_DATA_TYPES_WAVEFORM_H_

#include <cstdint>
#include <vector>

namespace spleeter
{
using Waveform = std::vector<std::vector<std::uint8_t>>;

}  // namespace spleeter
#endif  /// SPLEETER_DATA_TYPES_WAVEFORM_H_
