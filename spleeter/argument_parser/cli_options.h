///
/// @file
/// @brief Contains command line interface options definitions
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_ARGUMENT_PARSER_CLI_OPTIONS_H_
#define SPLEETER_ARGUMENT_PARSER_CLI_OPTIONS_H_

#include <string>

namespace spleeter
{
/// @brief Contains Command Line Interface (CLI) Options
struct CLIOptions
{
    /// @brief List of input audio filenames
    std::string inputs{"external/audio_example/file/audio_example.wav"};

    /// @brief Path of the output directory to write audio files in
    std::string output_path{"separated_audio"};

    /// @brief Template string that will be formatted to generated
    /// output filename. Such template should be Python formattable
    /// string, and could use {filename}, {instrument}, and {codec}
    /// variables
    std::string filename_format{"{filename}/{instrument}.{codec}"};

    /// @brief JSON filename that contains params
    /// choices: { "spleeter:5stems", "spleeter:4stems", "spleeter:2stems" }
    std::string configuration{"spleeter:5stems"};

    /// @brief Set the starting offset to separate audio from
    double offset{0.0};

    /// @brief Set a maximum duration for the processing audio
    /// (only separate offset + duration first seconds of the input file)
    double duration{600.0};

    /// @brief Audio codec to be used for separate output
    /// @todo v1.2 supports only *.wav
    /// choices: { wav, mp3, ogg, m4a, wma, flac }
    std::string codec{"wav"};

    /// @brief Audio bitrate to be used for separate output
    std::int32_t bitrate{128000};

    /// @brief Whether to use multichannel Wiener filtering for separation
    bool mwf{false};

    /// @brief Path to folder with musDB
    std::string mus_dir{};

    /// @brief Path of the folder containing audio data for training
    std::string audio_path{};

    /// @brief Name of the audio adapater to use for audio I/O
    std::string audio_adapter{"audionamix"};

    /// @brief Shows verbose logs
    bool verbose{false};
};

}  // namespace spleeter
#endif  /// SPLEETER_ARGUMENT_PARSER_CLI_OPTIONS_H_
