///
/// @file
/// @brief Contains component tests for Spleeter
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/argument_parser.h"
#include "spleeter/argument_parser/i_argument_parser.h"
#include "spleeter/spleeter.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

namespace spleeter
{
namespace
{
TEST(SpleeterSpec, Constructor)
{
    std::unique_ptr<IArgumentParser> argument_parser{std::make_unique<ArgumentParser>()};
    Spleeter unit(std::move(argument_parser));
}

}  // namespace
}  // namespace spleeter