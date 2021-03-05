// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/filtering/one_pole.h"
#include "gtest/gtest.h"

using namespace ha::dtb::filtering;

//------------------------------------------------------------------------
//	FrequencyModulatorTest
//------------------------------------------------------------------------
TEST(OnePoleTest, testOnePoleInitialisation)
{
    auto data = one_pole_filter::init();
    EXPECT_FLOAT_EQ(data.a, 0.9);
    EXPECT_FLOAT_EQ(data.b, 0.1);
    EXPECT_FLOAT_EQ(data.z, 0.0);
}

//------------------------------------------------------------------------