///
/// @file
/// @brief Contains Argument Parser Interface class
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_ARGUMENT_PARSER_I_ARGUMENT_PARSER_H_
#define SPLEETER_ARGUMENT_PARSER_I_ARGUMENT_PARSER_H_

#include "spleeter/argument_parser/cli_options.h"

namespace spleeter
{
/// @brief Argument Parser Interface class
class IArgumentParser
{
  public:
    /// @brief Destructor
    virtual ~IArgumentParser() = default;

    /// @brief Provides Parsed Arguments
    virtual CLIOptions GetParsedArgs() const = 0;

  protected:
    /// @brief Parse Arguments from argc, argv
    virtual CLIOptions ParseArgs(int argc, char** argv) = 0;
};
}  // namespace spleeter

#endif  /// SPLEETER_ARGUMENT_PARSER_I_ARGUMENT_PARSER_H_
