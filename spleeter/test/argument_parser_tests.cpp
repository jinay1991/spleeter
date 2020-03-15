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

    EXPECT_EQ(actual.loop_count, 1);
    EXPECT_EQ(actual.max_profiling_buffer_entries, 1024);
    EXPECT_EQ(actual.number_of_results, 5);
    EXPECT_EQ(actual.number_of_threads, 4);
    EXPECT_FLOAT_EQ(actual.input_mean, 127.5F);
    EXPECT_FLOAT_EQ(actual.input_std, 127.5F);
    EXPECT_FALSE(actual.verbose);
    EXPECT_FALSE(actual.profiling);
    EXPECT_FALSE(actual.input_floating);
    EXPECT_THAT(actual.input_name, ::testing::Eq("data/audio_example.mp3"));
    EXPECT_THAT(actual.labels_name, ::testing::Eq("data/labels.txt"));
    EXPECT_EQ(actual.model_name, "external/mobilenet_v2_1.0_224_quant/mobilenet_v2_1.0_224_quant.tflite");
    EXPECT_THAT(actual.result_directory, ::testing::Eq("results"));
}
TEST(ArgumentParserTest, WhenHelpArgument)
{
    char* argv[] = {"label_image", "-h"};
    int argc = sizeof(argv) / sizeof(char*);
    EXPECT_EXIT(ArgumentParser(argc, argv), ::testing::ExitedWithCode(1), "");
}

TEST(ArgumentParserTest, ParameterizedConstructor)
{
    char* argv[] = {"label_image",
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

    EXPECT_EQ(actual.loop_count, 2);
    EXPECT_EQ(actual.max_profiling_buffer_entries, 10);
    EXPECT_EQ(actual.number_of_results, 2);
    EXPECT_EQ(actual.number_of_threads, 1);
    EXPECT_FLOAT_EQ(actual.input_mean, 1.0F);
    EXPECT_FLOAT_EQ(actual.input_std, 10.0F);
    EXPECT_TRUE(actual.verbose);
    EXPECT_TRUE(actual.profiling);
    EXPECT_FALSE(actual.input_floating);
    EXPECT_THAT(actual.input_name, ::testing::Eq("test.jpg"));
    EXPECT_THAT(actual.model_name, ::testing::Eq("test_model.tflite"));
    EXPECT_THAT(actual.labels_name, ::testing::Eq("labels.txt"));
    EXPECT_THAT(actual.result_directory, ::testing::Eq("data/intermediate_tensors"));
}
}  // namespace
}  // namespace spleeter