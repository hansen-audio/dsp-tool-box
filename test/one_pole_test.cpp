// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/filtering/one_pole.h"
#include "gtest/gtest.h"

using namespace ha::dtb::filtering;

//------------------------------------------------------------------------
//	FrequencyModulatorTest
//------------------------------------------------------------------------
TEST(OnePoleTest, testOnePoleInitialisation)
{
    auto opf_context = one_pole_filter::create();
    EXPECT_FLOAT_EQ(opf_context.a, 0.9);
    EXPECT_FLOAT_EQ(opf_context.b, 0.1);
    EXPECT_FLOAT_EQ(opf_context.z, 0.0);
}

//------------------------------------------------------------------------