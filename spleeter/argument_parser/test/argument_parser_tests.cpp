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

    EXPECT_EQ(actual.inputs, "external/audio_example/file/audio_example.wav");
    EXPECT_EQ(actual.output_path, "separated_audio");
    EXPECT_EQ(actual.filename_format, "{filename}/{instrument}.{codec}");
    EXPECT_EQ(actual.configuration, "spleeter:5stems");
    EXPECT_DOUBLE_EQ(actual.offset, 0.0);
    EXPECT_DOUBLE_EQ(actual.duration, 600.0);
    EXPECT_EQ(actual.codec, "wav");
    EXPECT_EQ(actual.bitrate, 128000);
    EXPECT_FALSE(actual.mwf);
    EXPECT_TRUE(actual.mus_dir.empty());
    EXPECT_TRUE(actual.audio_path.empty());
    EXPECT_TRUE(actual.audio_adapter.empty());
    EXPECT_FALSE(actual.verbose);
}
TEST(ArgumentParserTest, WhenHelpArgument)
{
    char* argv[] = {"app", "-h"};
    int argc = sizeof(argv) / sizeof(char*);
    EXPECT_EXIT(ArgumentParser(argc, argv), ::testing::ExitedWithCode(1), "");
}

TEST(ArgumentParserTest, ParameterizedConstructor)
{
    // clang-format off
    char* argv[] = {"app",
                    "-i", "sample.mp3",
                    "-o", "audio_out",
                    "-f", "{filename}_{instrument}.{codec}",
                    "--params_filename", "spleeter:5stems",
                    "-s", "1.0",
                    "-d", "20",
                    "-c", "mp3",
                    "-b", "320k",
                    "-m", "1",
                    "-v", "1",
                    "-u", "my_mus_dir",
                    "-t", "data.file",
                    "-a", "adapter"};
    // clang-format on
    int argc = sizeof(argv) / sizeof(char*);
    auto unit = ArgumentParser(argc, argv);
    auto actual = unit.GetParsedArgs();

    EXPECT_EQ(actual.inputs, "sample.mp3");
    EXPECT_EQ(actual.output_path, "audio_out");
    EXPECT_EQ(actual.filename_format, "{filename}_{instrument}.{codec}");
    EXPECT_EQ(actual.configuration, "spleeter:5stems");
    EXPECT_DOUBLE_EQ(actual.offset, 1.0);
    EXPECT_DOUBLE_EQ(actual.duration, 20.0);
    EXPECT_EQ(actual.codec, "mp3");
    EXPECT_EQ(actual.bitrate, 320000);
    EXPECT_TRUE(actual.mwf);
    EXPECT_EQ(actual.mus_dir, "my_mus_dir");
    EXPECT_EQ(actual.audio_path, "data.file");
    EXPECT_EQ(actual.audio_adapter, "adapter");
    EXPECT_TRUE(actual.verbose);
}
}  // namespace
}  // namespace spleeter
