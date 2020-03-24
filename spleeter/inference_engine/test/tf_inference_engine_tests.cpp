///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/inference_engine/tf_inference_engine.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace spleeter
{
namespace
{
TEST(TFInferenceEngineTest, Construction)
{
    TFInferenceEngine unit_;
    unit_.Init();

    unit_.Execute();
}
}  // namespace
}  // namespace spleeter
