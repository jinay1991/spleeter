///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/argument_parser.h"
#include "spleeter/logging/logging.h"

#include <iostream>

namespace spleeter
{
namespace
{
void PrintUsage()
{
    LOG(INFO) << "spleeter\n"
              << "--count, -c: loop interpreter->Invoke() for certain times\n"
              << "--input_mean, -b: input mean\n"
              << "--result_directory, -d: directory path\n"
              << "--input_std, -s: input standard deviation\n"
              << "--image, -i: image_name.bmp\n"
              << "--max_profiling_buffer_entries, -e: maximum profiling buffer entries\n"
              << "--labels, -l: labels for the model\n"
              << "--tflite_model, -m: model_name.tflite\n"
              << "--profiling, -p: [0|1], profiling or not\n"
              << "--num_results, -r: number of results to show\n"
              << "--threads, -t: number of threads\n"
              << "--verbose, -v: [0|1] print more information\n"
              << "--save_results, -f: [0:1] save results in result_directory\n"
              << "--help, -h: print help\n";
}
}  // namespace
ArgumentParser::ArgumentParser() : cli_options_{} {}

ArgumentParser::ArgumentParser(int argc, char* argv[])
    : long_options_{{"count", required_argument, nullptr, 'c'},
                    {"image", required_argument, nullptr, 'i'},
                    {"input_mean", required_argument, nullptr, 'b'},
                    {"input_std", required_argument, nullptr, 's'},
                    {"labels", required_argument, nullptr, 'l'},
                    {"max_profiling_buffer_entries", required_argument, nullptr, 'e'},
                    {"num_results", required_argument, nullptr, 'r'},
                    {"profiling", required_argument, nullptr, 'p'},
                    {"tflite_model", required_argument, nullptr, 'm'},
                    {"threads", required_argument, nullptr, 't'},
                    {"verbose", required_argument, nullptr, 'v'},
                    {"result_directory", required_argument, nullptr, 'd'},
                    {"save_results", required_argument, nullptr, 'f'},
                    {"help", 0, nullptr, 'h'},
                    {nullptr, 0, nullptr, 0}},
      optstring_{"b:c:d:e:f:h:i:l:m:p:r:s:v:t:"}
{
    cli_options_ = ParseArgs(argc, argv);
}

ArgumentParser::~ArgumentParser() {}

CLIOptions ArgumentParser::GetParsedArgs() const { return cli_options_; }

CLIOptions ArgumentParser::ParseArgs(int argc, char* argv[])
{
    while (true)
    {
        std::int32_t c = 0;
        std::int32_t optindex = 0;

        c = getopt_long(argc, argv, optstring_.c_str(), long_options_.data(), &optindex);
        /* Detect the end of the options. */
        if (c == -1)
        {
            break;
        }

        switch (c)
        {
            case 'b':
                cli_options_.input_mean = strtod(optarg, nullptr);
                LOG(INFO) << "input_mean: " << cli_options_.input_mean;
                break;
            case 'c':
                cli_options_.loop_count = strtol(optarg, nullptr, 10);
                LOG(INFO) << "loop_count: " << cli_options_.loop_count;
                break;
            case 'd':
                cli_options_.result_directory = optarg;
                LOG(INFO) << "result_directory: " << cli_options_.result_directory;
                break;
            case 'e':
                cli_options_.max_profiling_buffer_entries = strtol(optarg, nullptr, 10);
                LOG(INFO) << "max_profiling_buffer_entries: " << cli_options_.max_profiling_buffer_entries;
                break;
            case 'f':
                cli_options_.save_results = strtol(optarg, nullptr, 10);
                LOG(INFO) << "save_results: " << cli_options_.save_results;
                break;
            case 'i':
                cli_options_.input_name = optarg;
                LOG(INFO) << "input_name: " << cli_options_.input_name;
                break;
            case 'l':
                cli_options_.labels_name = optarg;
                LOG(INFO) << "labels_name: " << cli_options_.labels_name;
                break;
            case 'm':
                cli_options_.model_name = optarg;
                LOG(INFO) << "model_name: " << cli_options_.model_name;
                break;
            case 'p':
                cli_options_.profiling = strtol(optarg, nullptr, 10);
                LOG(INFO) << "profiling: " << cli_options_.profiling;
                break;
            case 'r':
                cli_options_.number_of_results = strtol(optarg, nullptr, 10);
                LOG(INFO) << "number_of_results: " << cli_options_.number_of_results;
                break;
            case 's':
                cli_options_.input_std = strtod(optarg, nullptr);
                LOG(INFO) << "input_std: " << cli_options_.input_std;
                break;
            case 't':
                cli_options_.number_of_threads = strtol(optarg, nullptr, 10);
                LOG(INFO) << "number_of_threads: " << cli_options_.number_of_threads;
                break;
            case 'v':
                cli_options_.verbose = strtol(optarg, nullptr, 10);
                LOG(INFO) << "verbose: " << cli_options_.verbose;
                break;
            case 'h':
            case '?':
                /* getopt_long already printed an error message. */
                PrintUsage();
            default:
                exit(1);
        }
    }
    return cli_options_;
}

}  // namespace spleeter