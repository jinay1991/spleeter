///
/// @file
/// @brief Contains High Level API/Class for Spleeter application
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_SPLEETER_H
#define SPLEETER_SPLEETER_H

#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/i_separator.h"

#include <memory>

namespace spleeter
{
/// @brief Contains High Level APIs for performing Spleeter tasks
class Spleeter
{
  public:
    /// @brief Constructor.
    explicit Spleeter(const CLIOptions& cli_options);

    /// @brief Initialize Spleeter
    void Init();

    /// @brief Execute Spleeter
    void Execute();

    /// @brief Shutdown Spleeter (release any occupied resources)
    void Shutdown();

  private:
    /// @brief Parsed Command Line options
    CLIOptions cli_options_;

    /// @brief Audio Separator
    std::unique_ptr<ISeparator> separator_;
};
}  // namespace spleeter

#endif  /// SPLEETER_SPLEETER_H
