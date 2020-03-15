///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/spleeter.h"
#include "spleeter/logging/logging.h"
#include "spleeter/separator.h"

namespace spleeter
{
Spleeter::Spleeter(std::unique_ptr<IArgumentParser> argument_parser) : argument_parser_{std::move(argument_parser)} {}

void Spleeter::Init()
{
    const auto cli_options = argument_parser_->GetParsedArgs();

    separator_ = std::make_unique<Separator>(cli_options.configuration, cli_options.mwf);
    separator_->Separate(cli_options.audio_path);

    LOG(INFO) << "Successfully Initialized!!";
}
void Spleeter::Execute() {}
void Spleeter::Shutdown() {}

}  // namespace spleeter
