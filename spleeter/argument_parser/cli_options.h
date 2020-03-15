///
/// @file
/// @brief Contains command line interface options definitions
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_ARGUMENT_PARSER_CLI_OPTIONS_H_
#define SPLEETER_ARGUMENT_PARSER_CLI_OPTIONS_H_

#include <cstdint>
#include <string>

namespace spleeter
{
/// @brief Contains Command Line Interface (CLI) Options
struct CLIOptions
{
    /// @brief Is Model require FP32/FP16 input?
    bool input_floating = false;

    /// @brief Enable/Disable Profiling
    bool profiling = false;

    /// @brief Enable/Disable Verbose Logging (Prints more information)
    bool verbose = false;

    /// @brief Input Mean for Model
    float input_mean = 127.5f;

    /// @brief Input StdDev for Model
    float input_std = 127.5f;

    /// @brief Number of iterations to loop interpreter->Invoke() for certain times
    std::int32_t loop_count = 1;

    /// @brief Maximum Profiling Buffer Entries [Required when profiling is enabled.]
    std::int32_t max_profiling_buffer_entries = 1024;

    /// @brief Number of results to show
    std::int32_t number_of_results = 5;

    /// @brief Number of threads to be used for Inference
    std::int32_t number_of_threads = 4;

    /// @brief Input Layer Type ["uint8_t", "float32"]
    std::string input_layer_type = "uint8_t";

    /// @brief Input Image Path
    std::string input_name = "data/grace_hopper.jpg";

    /// @brief Labels for the model
    std::string labels_name = "data/labels.txt";

    /// @brief Model Path
    std::string model_name = "external/mobilenet_v2_1.0_224_quant/mobilenet_v2_1.0_224_quant.tflite";

    /// @brief Result Directory
    std::string result_directory = "results";

    /// @brief Enable/Disable for saving results
    bool save_results = false;
};

}  // namespace spleeter
#endif  /// SPLEETER_ARGUMENT_PARSER_CLI_OPTIONS_H_
