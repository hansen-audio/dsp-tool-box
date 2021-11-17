// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/filtering/one_pole.h"
#include "gtest/gtest.h"

using namespace ha::dtb::filtering;

/**
 * @brief one_pole_test
 */
TEST(one_pole_test, test_one_pole_initialisation)
{
    auto one_pole = OnePoleImpl::create();
    EXPECT_FLOAT_EQ(one_pole.a, 0.9);
    EXPECT_FLOAT_EQ(one_pole.b, 0.1);
    EXPECT_FLOAT_EQ(one_pole.z, 0.0);
}

//-----------------------------------------------------------------------------