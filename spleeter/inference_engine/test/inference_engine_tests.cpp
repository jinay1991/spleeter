///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/inference_engine/inference_engine.h"
#include "spleeter/inference_engine/tf_inference_engine.h"
#include "spleeter/inference_engine/tflite_inference_engine.h"
#include "spleeter/inference_engine/torch_inference_engine.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <string>

namespace spleeter
{
namespace
{
template <typename T>
class InferenceEngineTest : public ::testing::Test
{
  public:
    InferenceEngineTest()
        : test_input_{"external/audio_example/file/audio_example.wav"}, test_waveform_{}, test_waveform_properties_{}
    {
    }

  protected:
    void SetUp() override
    {
        const std::string configuration = "spleeter:5stems";

        AudionamixAudioAdapter audio_adapter{};
        this->test_waveform_ = audio_adapter.Load(this->test_input_, 0, -1, 44100);
        this->test_waveform_properties_ = audio_adapter.GetProperties();

        this->unit_ = std::make_unique<T>(configuration);
        this->unit_->Init();
        this->unit_->SetInputWaveform(this->test_waveform_,
                                      this->test_waveform_properties_.nb_frames,
                                      this->test_waveform_properties_.nb_channels);
    }

    void TearDown() override { unit_->Shutdown(); }

    const std::string test_input_;
    Waveform test_waveform_;
    AudioProperties test_waveform_properties_;

    std::unique_ptr<IInferenceEngine> unit_;
};
TYPED_TEST_SUITE_P(InferenceEngineTest);

TYPED_TEST_P(InferenceEngineTest, Sanity)
{
    this->unit_->Execute();

    auto actual = this->unit_->GetResults();
    EXPECT_EQ(5U, actual.size());
}

REGISTER_TYPED_TEST_SUITE_P(InferenceEngineTest, Sanity);

typedef ::testing::Types<TFInferenceEngine, /*TFLiteInferenceEngine,*/ TorchInferenceEngine> InferenceEngineTestTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(TypeTests, InferenceEngineTest, InferenceEngineTestTypes);

class InferenceEngineTypeTest : public ::testing::TestWithParam<InferenceEngineType>
{
  public:
    InferenceEngineTypeTest() : configuration_{"spleeter:5stems"}, unit_{} {}

  protected:
    const std::string configuration_;
    InferenceEngine unit_;
};
TEST_P(InferenceEngineTypeTest, GivenInferenceEngine_ExpectSelectedEngine)
{
    unit_.SelectInferenceEngine(GetParam(), configuration_);

    EXPECT_EQ(GetParam(), unit_.GetType());
}
INSTANTIATE_TEST_CASE_P(TypeTests,
                        InferenceEngineTypeTest,
                        ::testing::Values(InferenceEngineType::kTensorFlow,
                                          InferenceEngineType::kTensorFlowLite,
                                          InferenceEngineType::kTorch));
}  // namespace
}  // namespace spleeter
