///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/spleeter.h"

namespace spleeter
{
Spleeter::Spleeter(std::unique_ptr<IArgumentParser> argument_parser) : argument_parser_{std::move(argument_parser)} {}

void Spleeter::Init() {}
void Spleeter::Execute() {}
void Spleeter::Shutdown() {}

}  // namespace spleeter