///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/argument_parser.h"
#include "spleeter/argument_parser/i_argument_parser.h"
#include "spleeter/logging/logging.h"
#include "spleeter/spleeter.h"

#include <memory>

int main(int argc, char** argv)
{
    try
    {
        std::unique_ptr<spleeter::IArgumentParser> argument_parser =
            std::make_unique<spleeter::ArgumentParser>(argc, argv);
        auto spleeter = std::make_unique<spleeter::Spleeter>(argument_parser->GetParsedArgs());
        spleeter->Init();

        spleeter->Execute();

        spleeter->Shutdown();
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Caught Exception!! " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
