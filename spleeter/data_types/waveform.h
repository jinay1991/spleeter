///
/// @file
/// @brief Contains Data Structure to hold Waveform (i.e. audio samples)
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_DATA_TYPES_WAVEFORM_H_
#define SPLEETER_DATA_TYPES_WAVEFORM_H_

#include <algorithm>
#include <cstdint>
#include <ostream>
#include <vector>

namespace spleeter
{
/// @brief Waveform (List of samples)
using Waveform = std::vector<float>;

/// @brief List of waveforms
using Waveforms = std::vector<Waveform>;

/// @brief Provide output stream for waveform (list of samples), prints number of samples it holds.
inline std::ostream& operator<<(std::ostream& out, const Waveform& waveform)
{
    out << "Waveform{nb_samples: " << waveform.size() << "}";
    return out;
}

/// @brief Provide output stream for waveforms (list of waveform), prints number of sample in each waveform
inline std::ostream& operator<<(std::ostream& out, const Waveforms& waveforms)
{
    std::int32_t idx{0};
    out << "Waveforms{\n";
    std::for_each(waveforms.begin(), waveforms.end(),
                  [&](const auto& waveform) { out << "  " << idx++ << ". " << waveform << "\n"; });
    out << "}";
    return out;
}
}  // namespace spleeter
#endif  /// SPLEETER_DATA_TYPES_WAVEFORM_H_
