///
/// @file
/// @brief Contains unit tests for Argument Parser APIs
/// @copyright Copyright (c) 2020. All Rights Reserved.
///
#include "spleeter/argument_parser/argument_parser.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

namespace spleeter
{
namespace
{
TEST(ArgumentParserTest, DefaultConstructor)
{
    auto unit = ArgumentParser();
    auto actual = unit.GetParsedArgs();

    EXPECT_EQ(actual.inputs, "");
    EXPECT_EQ(actual.output_path, "separated_audio");
    EXPECT_EQ(actual.filename_format, "{filename}/{instrument}.{codec}");
    EXPECT_EQ(actual.configuration, "spleeter:2stems");
    EXPECT_DOUBLE_EQ(actual.offset, 0.0);
    EXPECT_DOUBLE_EQ(actual.duration, 600.0);
    EXPECT_EQ(actual.codec, "wav");
    EXPECT_EQ(actual.bitrate, "128k");
    EXPECT_EQ(actual.mwf, false);
    EXPECT_EQ(actual.mus_dir, "");
    EXPECT_EQ(actual.audio_path, "");
    EXPECT_EQ(actual.audio_adapter, "");
    EXPECT_EQ(actual.verbose, false);
}
TEST(ArgumentParserTest, WhenHelpArgument)
{
    char* argv[] = {"app", "-h"};
    int argc = sizeof(argv) / sizeof(char*);
    EXPECT_EXIT(ArgumentParser(argc, argv), ::testing::ExitedWithCode(1), "");
}
TEST(ArgumentParserTest, WhenUnknownArgument)
{
    char* argv[] = {"app", "-x"};
    int argc = sizeof(argv) / sizeof(char*);
    EXPECT_EXIT(ArgumentParser(argc, argv), ::testing::ExitedWithCode(1), "");
}

TEST(ArgumentParserTest, DISABLED_ParameterizedConstructor)
{
    char* argv[] = {"app",
                    "-m",
                    "test_model.tflite",
                    "-f",
                    "1",
                    "-p",
                    "1",
                    "--image",
                    "test.jpg",
                    "-l",
                    "labels.txt",
                    "-c",
                    "2",
                    "-e",
                    "10",
                    "-r",
                    "2",
                    "-t",
                    "1",
                    "-v",
                    "1",
                    "-d",
                    "data/intermediate_tensors",
                    "-s",
                    "10.0",
                    "-b",
                    "1"};
    int argc = sizeof(argv) / sizeof(char*);
    auto unit = ArgumentParser(argc, argv);
    auto actual = unit.GetParsedArgs();

    EXPECT_EQ(actual.inputs, "");
    EXPECT_EQ(actual.output_path, "separated_audio");
    EXPECT_EQ(actual.filename_format, "{filename}/{instrument}.{codec}");
    EXPECT_EQ(actual.configuration, "spleeter:2stems");
    EXPECT_DOUBLE_EQ(actual.offset, 0.0);
    EXPECT_DOUBLE_EQ(actual.duration, 600.0);
    EXPECT_EQ(actual.codec, "wav");
    EXPECT_EQ(actual.bitrate, "128k");
    EXPECT_EQ(actual.mwf, false);
    EXPECT_EQ(actual.mus_dir, "");
    EXPECT_EQ(actual.audio_path, "");
    EXPECT_EQ(actual.audio_adapter, "");
    EXPECT_EQ(actual.verbose, false);
}
}  // namespace
}  // namespace spleeter