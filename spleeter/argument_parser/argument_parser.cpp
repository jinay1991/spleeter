///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/argument_parser.h"

#include "spleeter/logging/logging.h"

namespace spleeter
{
namespace
{
void PrintUsage()
{
    LOG(INFO) << "spleeter\n"
              << "--inputs, -i: List of input audio filenames\n"
              << "--output_path, -o: Path of the output directory to write audio files in\n"
              << "--filename_format, -f: Template string that will be formatted to generated\n"
                 "                       output filename. Such template should be Python formattable\n"
                 "                       string, and could use {filename}, {instrument}, and {codec} variables\n"
              << "--duration, -d: Set a maximum duration for the processing audio\n"
                 "                (only separate offset + duration first seconds of the input file)\n"
              << "--offset, -s: Set the starting offset to separate audio from\n"
              << "--codec, -c: Audio codec to be used for separate output\n"
                 "             choices: { wav, mp3, ogg, m4a, wma, flac }\n"
              << "--bitrate, -b: Audio bitrate to be used for separate output\n"
              << "--mwf, -m: [0|1] Whether to use multichannel Wiener filtering for separation\n"
              << "--mus_dir, -u: Path to folder with musDB\n"
              << "--adapter, -a: Name of the audio adapater to use for audio I/O\n"
              << "--params_filename, -p: JSON filename that contains params\n"
              << "--verbose, -v: [0|1] Shows verbose logs\n"
              << "--data, -t: Path of the folder containing audio data for training\n"
              << "--help, -h: print help\n";
}
}  // namespace
ArgumentParser::ArgumentParser() : cli_options_{} {}

ArgumentParser::ArgumentParser(int argc, char* argv[])
    : long_options_{{"inputs", required_argument, nullptr, 'i'},
                    {"output_path", required_argument, nullptr, 'o'},
                    {"filename_format", required_argument, nullptr, 'f'},
                    {"duration", required_argument, nullptr, 'd'},
                    {"offset", required_argument, nullptr, 's'},
                    {"codec", required_argument, nullptr, 'c'},
                    {"bitrate", required_argument, nullptr, 'b'},
                    {"mwf", required_argument, nullptr, 'm'},
                    {"mus_dir", required_argument, nullptr, 'u'},
                    {"adapter", required_argument, nullptr, 'a'},
                    {"params_filename", required_argument, nullptr, 'p'},
                    {"verbose", required_argument, nullptr, 'v'},
                    {"data", required_argument, nullptr, 't'},
                    {"help", 0, nullptr, 'h'},
                    {nullptr, 0, nullptr, 0}},
      optstring_{"a:b:c:d:f:h:i:m:p:o:s:t:u:v:"}
{
    cli_options_ = ParseArgs(argc, argv);
}

ArgumentParser::~ArgumentParser() {}

CLIOptions ArgumentParser::GetParsedArgs() const
{
    return cli_options_;
}

CLIOptions ArgumentParser::ParseArgs(int argc, char* argv[])
{
    if (argc > 1)
    {
        LOG(INFO) << "Parsed arguments: ";
    }
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
            case 'i':
                cli_options_.inputs = optarg;
                LOG(INFO) << " (+) inputs: " << cli_options_.inputs;
                break;
            case 'o':
                cli_options_.output_path = optarg;
                LOG(INFO) << " (+) output_path: " << cli_options_.output_path;
                break;
            case 'f':
                cli_options_.filename_format = optarg;
                LOG(INFO) << " (+) filename_format: " << cli_options_.filename_format;
                break;
            case 'd':
                cli_options_.duration = strtod(optarg, nullptr);
                LOG(INFO) << " (+) duration: " << cli_options_.duration;
                break;
            case 's':
                cli_options_.offset = strtod(optarg, nullptr);
                LOG(INFO) << " (+) offset: " << cli_options_.offset;
                break;
            case 'c':
                cli_options_.codec = optarg;
                LOG(INFO) << " (+) codec: " << cli_options_.codec;
                break;
            case 'b':
                cli_options_.bitrate = std::stoi(optarg) * 1000;
                LOG(INFO) << " (+) bitrate: " << cli_options_.bitrate;
                break;
            case 'm':
                cli_options_.mwf = strtol(optarg, nullptr, 10);
                LOG(INFO) << " (+) mwf: " << std::boolalpha << cli_options_.mwf;
                break;
            case 'u':
                cli_options_.mus_dir = optarg;
                LOG(INFO) << " (+) mus_dir: " << cli_options_.mus_dir;
                break;
            case 'a':
                cli_options_.audio_adapter = optarg;
                LOG(INFO) << " (+) audio_adapter: " << cli_options_.audio_adapter;
                break;
            case 'p':
                cli_options_.configuration = optarg;
                LOG(INFO) << " (+) configuration: " << cli_options_.configuration;
                break;
            case 'v':
                cli_options_.verbose = strtol(optarg, nullptr, 10);
                LOG(INFO) << " (+) verbose: " << std::boolalpha << cli_options_.verbose;
                break;
            case 't':
                cli_options_.audio_path = optarg;
                LOG(INFO) << " (+) audio_path: " << cli_options_.audio_path;
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
