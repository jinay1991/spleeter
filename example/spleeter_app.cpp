///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/spleeter.h"

#include <iostream>
#include <memory>

int main(void)
{
    try
    {
        /// Initialize
        auto cli_options = spleeter::CLIOptions{};
        cli_options.inputs = std::string{"external/audio_example/file/audio_example.wav"};
        cli_options.output_path = std::string{"separated_audio"};
        cli_options.configuration = std::string{"spleeter:5stems"};

        cli_options.audio_adapter = std::string{"audionamix"};
        cli_options.codec = std::string{"wav"};
        cli_options.bitrate = 192000;

        auto spleeter = std::make_unique<spleeter::Spleeter>(cli_options);
        spleeter->Init();

        /// Run
        spleeter->Execute();

        /// Deinitialize
        spleeter->Shutdown();
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught Exception!! " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
