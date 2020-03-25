///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/inference_engine/tf_inference_engine.h"
#include "spleeter/logging/logging.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <wave/file.h>

#include <algorithm>
#include <string>
#include <vector>

namespace spleeter
{
namespace
{
TEST(Loader, DISABLED_Sandbox)
{
    auto cli_options = CLIOptions{};
    TFInferenceEngine unit{cli_options};

    unit.Init();

    // Read wav file
    wave::File file;
    file.Open(std::string(cli_options.inputs), wave::OpenMode::kIn);
    std::error_code err;
    auto data = file.Read(err);
    ASSERT_CHECK(!err) << "Couldn't read input file...";

    const auto nb_frames = static_cast<std::int32_t>(file.frame_number());
    const auto nb_channels = static_cast<std::int32_t>(file.channel_number());
    unit.SetInputWaveform(data, nb_frames, nb_channels);

    unit.Execute();

    // std::vector<tensorflow::Tensor> output;
    // if (!bundle.session->Run({{"Placeholder", input}}, {"strided_slice_13", "strided_slice_23"}, {}, &output).ok())
    // {
    //     std::cerr << "Run Failed" << std::endl;
    //     return;
    // }

    // // write data in a std::vector<float>
    // std::vector<float> result_data(data.size());  // same size as input
    // auto vocals = output[0].matrix<float>().data();
    // //  auto acompaniments = output[1].matrix<float>().data();
    // std::copy(vocals, vocals + data.size(), result_data.data());

    // auto waveforms = unit.GetResults();
    // std::vector<float> vocals{};
    // std::transform(waveforms.begin(), waveforms.end(), std::back_inserter(vocals),
    //                [&](const auto& value) { return static_cast<float>(value); });

    // wave::File result;
    // result.Open(cli_options.output_path + "/vocals.wav", wave::OpenMode::kOut);
    // result.set_sample_rate(44100);
    // result.set_channel_number(2);
    // result.Write(vocals);
    // ASSERT_CHECK(!err) << "Couldn't write the output file";
}
TEST(TFInferenceEngineTest, DISABLED_DefaultConstructor)
{
    auto unit = TFInferenceEngine{};
    unit.Init();
    unit.SetInputWaveform(Waveform{}, 0, 0);

    unit.Execute();

    auto actual = unit.GetResults();
    EXPECT_THAT(5U, actual.size());
}

TEST(TFInferenceEngineTest, DISABLED_GivenDefaultConfiguration_ExpectTwoResultantWaveforms)
{
    auto cli_options = CLIOptions{};
    cli_options.configuration = "spleeter:4stems";
    auto unit = TFInferenceEngine{cli_options};
    unit.Init();
    unit.SetInputWaveform(Waveform{}, 0, 0);

    unit.Execute();
    auto actual = unit.GetResults();
    EXPECT_THAT(4U, actual.size());
}

}  // namespace
}  // namespace spleeter
