///
/// @file logging_test.cpp
/// @brief Contains unit tests for Logging APIs
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/logging/logging.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace spleeter
{
namespace logging
{
namespace
{
TEST(LoggingWrapperTest, AssertionMacro) { EXPECT_EXIT(ASSERT_CHECK(false), ::testing::KilledBySignal(SIGABRT), ""); }
TEST(LoggingWrapperTest, AssertionCompareMacro)
{
    EXPECT_EXIT(ASSERT_CHECK_EQ(1, 2), ::testing::KilledBySignal(SIGABRT), "");
}

TEST(LoggingWrapperTest, FatalLogging)
{
    auto unit = new LoggingWrapper(LoggingWrapper::LogSeverity::FATAL, true);
    unit->Stream() << "test";
    EXPECT_EXIT(delete unit, ::testing::KilledBySignal(SIGABRT), "");
}
TEST(LoggingWrapperTest, NoLogging)
{
    ::testing::internal::CaptureStderr();
    ::testing::internal::CaptureStdout();
    LoggingWrapper(LoggingWrapper::LogSeverity::DEBUG, false).Stream() << "test";
    LoggingWrapper(LoggingWrapper::LogSeverity::INFO, false).Stream() << "test";
    LoggingWrapper(LoggingWrapper::LogSeverity::ERROR, false).Stream() << "test";
    LoggingWrapper(LoggingWrapper::LogSeverity::WARN, false).Stream() << "test";
    EXPECT_TRUE(::testing::internal::GetCapturedStdout().empty());
    EXPECT_TRUE(::testing::internal::GetCapturedStderr().empty());
}
TEST(LoggingWrapperTest, OnStandardOutput)
{
    ::testing::internal::CaptureStderr();
    ::testing::internal::CaptureStdout();
    LoggingWrapper(LoggingWrapper::LogSeverity::DEBUG, true).Stream() << "test";
    LoggingWrapper(LoggingWrapper::LogSeverity::INFO, true).Stream() << "test";
    LoggingWrapper(LoggingWrapper::LogSeverity::WARN, true).Stream() << "test";
    EXPECT_FALSE(::testing::internal::GetCapturedStdout().empty());
    EXPECT_TRUE(::testing::internal::GetCapturedStderr().empty());
}
TEST(LoggingWrapperTest, OnStandardError)
{
    ::testing::internal::CaptureStderr();
    ::testing::internal::CaptureStdout();
    LoggingWrapper(LoggingWrapper::LogSeverity::ERROR, true).Stream() << "test";
    EXPECT_TRUE(::testing::internal::GetCapturedStdout().empty());
    EXPECT_FALSE(::testing::internal::GetCapturedStderr().empty());
}
}  // namespace
}  // namespace logging
}  // namespace spleeter