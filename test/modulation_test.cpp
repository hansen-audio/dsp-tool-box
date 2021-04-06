// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/modulation/modulation_phase.h"
#include "gtest/gtest.h"

//------------------------------------------------------------------------
//	FrequencyModulatorTest
//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testFreeRunningOverflowInOneStep)
{
    using phase      = ha::dtb::modulation::phase;
    auto phase_value = phase::value_type(0.);
    phase::context_t context;
    phase::set_sample_rate(context, 44100.f);
    phase::set_mode(context, phase::modes::MODE_FREE);
    phase::set_rate(context, 1);
    phase::update(context, phase_value, 44100);
    EXPECT_TRUE(phase::update(context, phase_value, 44100));
    EXPECT_EQ(phase_value, 0.f);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testFreeRunningOverflowInManyStep)
{
    using phase      = ha::dtb::modulation::phase;
    auto phase_value = phase::value_type(0.);
    phase::context_t context;
    phase::set_sample_rate(context, 44100.f);
    phase::set_mode(context, phase::modes::MODE_FREE);
    phase::set_rate(context, 1);

    const auto offsetInSamplesRoundingErrors = 23;
    auto counter                             = 44100 + offsetInSamplesRoundingErrors;
    bool overflow                            = false;
    while (counter-- > 0)
        overflow = phase::update(context, phase_value, 1);

    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testTempoSyncedOverflowInOneStep)
{
    using phase      = ha::dtb::modulation::phase;
    auto phase_value = phase::value_type(0.);
    phase::context_t context;
    phase::set_sample_rate(context, 44100.f);
    phase::set_mode(context, phase::modes::MODE_TEMPO_SYNC);
    phase::set_tempo(context, 120.f);
    phase::set_note_length(context, 1.f);

    EXPECT_TRUE(phase::update(context, phase_value, 44100 * 2)); // 1 Note takes 2 seconds at 120BPM
    EXPECT_EQ(phase_value, 0.f);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testTempoSyncedOverflowInManyStep)
{
    using phase      = ha::dtb::modulation::phase;
    auto phase_value = phase::value_type(0.);
    phase::context_t context;
    phase::set_sample_rate(context, 44100.f);
    phase::set_mode(context, phase::modes::MODE_TEMPO_SYNC);
    phase::set_tempo(context, 120.f);
    phase::set_note_length(context, 1.f);

    const auto offsetInSamplesRoundingErrors = 37;
    auto counter                             = (44100 + offsetInSamplesRoundingErrors) * 2;
    bool overflow                            = false;
    while (counter-- > 0)
        overflow = phase::update(context, phase_value, 1);

    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testProjectSyncedOverflowInOneStep)
{
    using phase      = ha::dtb::modulation::phase;
    auto phase_value = phase::value_type(0.);
    phase::context_t context;
    phase::set_sample_rate(context, 44100.f);
    phase::set_mode(context, phase::modes::MODE_PROJECT_SYNC);
    phase::set_tempo(context, 120.f);
    phase::set_note_length(context, 1.f);

    phase::set_project_time(context, 4.f);
    bool overflow = phase::update(context, phase_value, 1);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(phase_value, 0.f);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testProjectSyncedTwoOverflowInOneStep)
{
    using phase      = ha::dtb::modulation::phase;
    auto phase_value = phase::value_type(0.);
    phase::context_t context;
    phase::set_sample_rate(context, 44100.f);
    phase::set_mode(context, phase::modes::MODE_PROJECT_SYNC);
    phase::set_tempo(context, 120.f);
    phase::set_note_length(context, 1.f);

    phase::set_project_time(context, 8.f);
    bool overflow = phase::update(context, phase_value, 1);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(phase_value, 0.f);
}