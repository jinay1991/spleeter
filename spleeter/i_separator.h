///
/// @file
/// @brief Contains interface definitions for Separator.
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_I_SEPARATOR_H_
#define SPLEETER_I_SEPARATOR_H_

#include "spleeter/data_types/audio_properties.h"
#include "spleeter/data_types/waveform.h"

#include <string>
#include <vector>

namespace spleeter
{
/// @brief Interface for Separator APIs
class ISeparator
{
  public:
    /// @brief Destructor
    virtual ~ISeparator() = default;

    /// @brief Performs source separation over the given waveform.
    ///
    /// The separation is performed synchronously but the result processing is done asynchronously, allowing for
    /// instance to export audio in parallel (through multiprocessing).
    ///
    /// Given result is passed by to the given consumer, which will be waited for task finishing if
    /// synchronous flag is True.
    ///
    /// @param waveform [in] Waveform to apply separation on.
    ///
    /// @returns Separated waveforms
    virtual Waveforms Separate(const Waveform& waveform, const AudioProperties& properties) = 0;

    /// @brief Performs source separation and export result to file using given audio adapter.
    ///
    /// Filename format should be a Python formattable string that could use following parameters:
    /// {instrument}, {filename} and {codec}.
    ///
    /// @param audio_descriptor [in] - Describe song to separate, used by audio adapter to retrieve and load audio data,
    ///                               in case of file based audio adapter, such descriptor would be a file path.
    /// @param destination [in]      - Target directory to write output to.
    /// @param audio_adapter [in]    - Audio adapter to use for I/O.
    /// @param offset [in]           - Offset of loaded song.
    /// @param duration [in]         - Duration of loaded song.
    /// @param codec [in]            - Export codec.
    /// @param bitrate [in]          - Export bitrate.
    /// @param filename_format [in]  - Filename format.
    /// @param synchronous [in]      - True is should by synchronous.
    virtual void SeparateToFile(const std::string& audio_descriptor, const std::string& destination,
                                const std::string& audio_adapter, const double offset, const double duration,
                                const std::string& codec, const std::int32_t bitrate,
                                const std::string& filename_format, const bool synchronous) = 0;
};
}  // namespace spleeter

#endif  /// SPLEETER_I_SEPARATOR_H_
