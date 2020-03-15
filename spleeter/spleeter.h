///
/// @file
/// @brief Contains High Level API/Class for Spleeter application
/// @copyright Copyright (c) 2020, MIT License
///
#ifndef SPLEETER_SPLEETER_H_
#define SPLEETER_SPLEETER_H_

#include "spleeter/argument_parser/i_argument_parser.h"

#include <memory>

namespace spleeter
{
/// @brief Contains High Level APIs for performing Spleeter tasks
class Spleeter
{
  public:
    /// @brief Constructor.
    explicit Spleeter(std::unique_ptr<IArgumentParser> argument_parser);

    /// @brief Destructor.
    virtual ~Spleeter() = default;

    /// @brief Initialize Spleeter
    virtual void Init();

    /// @brief Execute Spleeter
    virtual void Execute();

    /// @brief Shutdown Spleeter (release any occupied resources)
    virtual void Shutdown();

  private:
    /// @brief Argument Parser
    std::unique_ptr<IArgumentParser> argument_parser_;
};
}  // namespace spleeter

#endif  /// SPLEETER_SPLEETER_H_