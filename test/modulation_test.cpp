// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/modulation/modulation_phase.h"
#include "gtest/gtest.h"

using real = ha::dtb::real;

//------------------------------------------------------------------------
//	FrequencyModulatorTest
//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testFreeRunningOverflowInOneStep)
{
    using phase      = ha::dtb::modulation::phase;
    auto phase_value = real(0.);
    phase::context cx;
    phase::set_sample_rate(cx, 44100.f);
    phase::set_sync_mode(cx, phase::sync_mode::FREE);
    phase::set_rate(cx, 1);
    phase::advance(cx, phase_value, 44100);

    bool const overflow = phase::advance(cx, phase_value, 44100);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(phase_value, 0.f);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testFreeRunningOverflowInManyStep)
{
    using phase      = ha::dtb::modulation::phase;
    auto phase_value = real(0.);
    phase::context cx;
    phase::set_sample_rate(cx, 44100.f);
    phase::set_sync_mode(cx, phase::sync_mode::FREE);
    phase::set_rate(cx, 1);

    const auto offsetInSamplesRoundingErrors = 23;
    auto counter  = 44100 + offsetInSamplesRoundingErrors;
    bool overflow = false;
    while (counter-- > 0)
        overflow = phase::advance(cx, phase_value, 1);

    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testTempoSyncedOverflowInOneStep)
{
    using phase       = ha::dtb::modulation::phase;
    auto phase_value  = real(0.);
    phase::context cx = phase::create();
    phase::set_note_length(cx, 1.f);

    // 1 Note takes 2 seconds at 120BPM
    bool const overflow = phase::advance(cx, phase_value, 44100 * 2);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(phase_value, 0.f);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testTempoSyncedOverflowInManyStep)
{
    using phase       = ha::dtb::modulation::phase;
    auto phase_value  = real(0.);
    phase::context cx = phase::create();
    phase::set_note_length(cx, 1.f);

    const auto offsetInSamplesRoundingErrors = 37;
    auto counter  = (44100 + offsetInSamplesRoundingErrors) * 2;
    bool overflow = false;
    while (counter-- > 0)
        overflow = phase::advance(cx, phase_value, 1);

    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testProjectSyncedOverflowInOneStep)
{
    using phase       = ha::dtb::modulation::phase;
    auto phase_value  = real(0.);
    phase::context cx = phase::create();
    phase::set_sync_mode(cx, phase::sync_mode::PROJECT_SYNC);
    phase::set_note_length(cx, 1.f);
    phase::set_project_time(cx, 4.f);

    bool overflow = phase::advance(cx, phase_value, 1);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(phase_value, 0.f);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testProjectSyncedTwoOverflowInOneStep)
{
    using phase       = ha::dtb::modulation::phase;
    auto phase_value  = real(0.);
    phase::context cx = phase::create();
    phase::set_sync_mode(cx, phase::sync_mode::PROJECT_SYNC);
    phase::set_note_length(cx, 1.f);
    phase::set_project_time(cx, 8.f);

    bool overflow = phase::advance(cx, phase_value, 1);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(phase_value, 0.f);
}