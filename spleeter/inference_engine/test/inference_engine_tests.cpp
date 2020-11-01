///
/// @file
/// @copyright Copyright (c) 2020. MIT License
///
#include "spleeter/audio/audionamix_audio_adapter.h"
#include "spleeter/datatypes/inference_engine.h"
#include "spleeter/inference_engine/i_inference_engine.h"
#include "spleeter/inference_engine/inference_engine_strategy.h"
#include "spleeter/inference_engine/null_inference_engine.h"
#include "spleeter/inference_engine/tf_inference_engine.h"
#include "spleeter/inference_engine/tflite_inference_engine.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <string>

namespace spleeter
{
namespace
{

using ::testing::AllOf;
using ::testing::Each;
using ::testing::Field;
using ::testing::Property;

template <typename T>
InferenceEngineParameters GetInferenceEngineParameter()
{
    return InferenceEngineParameters{};
}

template <>
InferenceEngineParameters GetInferenceEngineParameter<TFInferenceEngine>()
{
    return InferenceEngineParameters{
        "external/models/5stems/saved_model",
        "waveform",
        {"strided_slice_18", "strided_slice_38", "strided_slice_48", "strided_slice_28", "strided_slice_58"},
        "spleeter:5stems"};
}

template <>
InferenceEngineParameters GetInferenceEngineParameter<TFLiteInferenceEngine>()
{
    return InferenceEngineParameters{
        "external/models/5stems/5stems.tflite",
        "waveform",
        {"strided_slice_18", "strided_slice_38", "strided_slice_48", "strided_slice_28", "strided_slice_58"},
        "spleeter:5stems"};
}

template <typename T>
class InferenceEngineFixture_WithInferenceEngineType : public ::testing::Test
{
  public:
    InferenceEngineFixture_WithInferenceEngineType()
        : test_waveform_path_{"external/audio_example/file/audio_example.wav"},
          audio_adapter_{},
          test_waveform_{audio_adapter_.Load(test_waveform_path_, 0, -1, 44100)},
          inference_engine_parameters_{GetInferenceEngineParameter<T>()},
          unit_{std::make_unique<T>(inference_engine_parameters_)}
    {
    }

  protected:
    void SetUp() override { unit_->Init(); }
    void RunOnce() { unit_->Execute(test_waveform_); }
    void TearDown() override { unit_->Shutdown(); }

    Waveforms GetInferenceResults() const { return unit_->GetResults(); }
    InferenceEngineParameters GetInferenceParameters() const { return inference_engine_parameters_; }

    std::int32_t GetFrames() const { return test_waveform_.nb_frames; }

  private:
    const std::string test_waveform_path_;
    AudionamixAudioAdapter audio_adapter_;
    const Waveform test_waveform_;
    const InferenceEngineParameters inference_engine_parameters_;
    InferenceEnginePtr unit_;
};
TYPED_TEST_SUITE_P(InferenceEngineFixture_WithInferenceEngineType);

TYPED_TEST_P(InferenceEngineFixture_WithInferenceEngineType, InferenceEngine_GivenTypicalInputs_ExpectInferenceResults)
{
    // When
    this->RunOnce();

    // Then
    const auto actual = this->GetInferenceResults();
    EXPECT_EQ(this->GetInferenceParameters().output_tensor_names.size(), actual.size());
    EXPECT_THAT(actual,
                Each(AllOf(Field(&Waveform::nb_channels, 2),
                           Field(&Waveform::nb_frames, this->GetFrames()),
                           Field(&Waveform::data, Property(&std::vector<float>::size, this->GetFrames() * 2)))));
}

REGISTER_TYPED_TEST_SUITE_P(InferenceEngineFixture_WithInferenceEngineType,
                            InferenceEngine_GivenTypicalInputs_ExpectInferenceResults);

typedef ::testing::Types<TFInferenceEngine, TFLiteInferenceEngine, NullInferenceEngine> InferenceEngineTestTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(InferenceEngine,
                               InferenceEngineFixture_WithInferenceEngineType,
                               InferenceEngineTestTypes);

class InferenceEngineStrategyTest : public ::testing::TestWithParam<InferenceEngineType>
{
  public:
    InferenceEngineStrategyTest() : inference_engine_params_{}, unit_{} {}

  protected:
    InferenceEngineParameters inference_engine_params_;
    InferenceEngineStrategy unit_;
};
TEST_P(InferenceEngineStrategyTest, GivenInferenceEngine_ExpectSelectedEngine)
{
    unit_.SelectInferenceEngine(GetParam(), inference_engine_params_);

    EXPECT_EQ(GetParam(), unit_.GetInferenceEngineType());
}
INSTANTIATE_TEST_CASE_P(InferenceEngine,
                        InferenceEngineStrategyTest,
                        ::testing::Values(InferenceEngineType::kTensorFlow,
                                          InferenceEngineType::kTensorFlowLite,
                                          InferenceEngineType::kInvalid));
}  // namespace
}  // namespace spleeter
