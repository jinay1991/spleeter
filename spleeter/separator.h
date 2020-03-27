///
/// @file
/// @brief Contains class definition for Separator which implements class ISeparator.
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_SEPARATOR_H_
#define SPLEETER_SEPARATOR_H_

#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/audio/i_audio_adapter.h"
#include "spleeter/i_separator.h"
#include "spleeter/inference_engine/inference_engine.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace spleeter
{
/// @brief Perform separation
class Separator : public ISeparator
{
  public:
    /// @brief Constructor
    ///
    /// @param params_descriptor [in] - Descriptor for TF params to be used.
    /// @param mwf [in]               - (Optional) True if MWF should be used, False otherwise.
    explicit Separator(const std::string& configuration, const bool mwf = false);

    /// @brief Destructor.
    virtual ~Separator() = default;

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
    Waveforms Separate(const Waveform& waveform, const AudioProperties& properties) override;

    /// @brief Performs source separation and export result to file using given audio adapter.
    ///
    /// Filename format should be a Python formattable string that could use following parameters:
    /// {instrument}, {filename} and {codec}.
    ///
    /// @param audio_descriptor [in] - Describe song to separate, used by audio adapter to retrieve and load audio data,
    ///                               in case of file based audio adapter, such descriptor would be a file path.
    /// @param destination [in]      - Target directory to write output to.
    /// @param audio_adapter [in]    - (Optional) Audio adapter to use for I/O.
    /// @param offset [in]           - (Optional) Offset of loaded song.
    /// @param duration [in]         - (Optional) Duration of loaded song.
    /// @param codec [in]            - (Optional) Export codec.
    /// @param bitrate [in]          - (Optional) Export bitrate.
    /// @param filename_format [in]  - (Optional) Filename format.
    /// @param synchronous [in]      - (Optional) True is should by synchronous.
    void SeparateToFile(const std::string& audio_descriptor, const std::string& destination,
                        const std::string& audio_adapter, const double offset = 0.0, const double duration = 600.0,
                        const std::string& codec = "wav", const std::int32_t bitrate = 128000,
                        const std::string& filename_format = "{filename}/{instrument}.{codec}",
                        const bool synchronous = true) override;

  private:
    /// @brief Is mwf enabled?
    bool mwf_;

    /// @brief Audio R/W Adapter
    std::unique_ptr<IAudioAdapter> audio_adapter_;

    /// @brief Inference Engine
    InferenceEngine inference_engine_;

    /// @brief List of waveform names (used to save files)
    std::vector<std::string> waveform_name_;
};
}  // namespace spleeter

#endif  /// SPLEETER_SEPARATOR_H_
