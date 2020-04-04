///
/// @file
/// @brief Contains definitions for Spleeter class methods
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/spleeter.h"

#include "spleeter/logging/logging.h"
#include "spleeter/separator.h"

namespace spleeter
{
Spleeter::Spleeter(const CLIOptions& cli_options)
    : cli_options_{cli_options}, separator_{std::make_unique<Separator>(cli_options_.configuration, cli_options_.mwf)}
{
}

void Spleeter::Init() {}

void Spleeter::Execute()
{
    separator_->SeparateToFile(cli_options_.inputs,
                               cli_options_.output_path,
                               cli_options_.audio_adapter,
                               cli_options_.offset,
                               cli_options_.duration,
                               cli_options_.codec,
                               cli_options_.bitrate,
                               cli_options_.filename_format,
                               false);
    SPLEETER_LOG(INFO) << "Successfully executed spleeter!!";
}
void Spleeter::Shutdown() {}

}  // namespace spleeter
