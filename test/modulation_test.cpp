// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/modulation/modulation_phase.h"
#include "gtest/gtest.h"

//------------------------------------------------------------------------
//	FrequencyModulatorTest
//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testFreeRunningOverflowInOneStep)
{
    auto current_phase = ha::dtb::modulation::phase::value_type(0.);
    ha::dtb::modulation::phase tmp_phase;
    tmp_phase.set_sample_rate(44100.f);
    tmp_phase.set_mode(ha::dtb::modulation::phase::modes::MODE_FREE);
    tmp_phase.set_rate(1);
    tmp_phase.update(current_phase, 44100);

    EXPECT_TRUE(tmp_phase.update(current_phase, 44100));
    EXPECT_EQ(current_phase, 0.f);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testFreeRunningOverflowInManyStep)
{
    auto current_phase = ha::dtb::modulation::phase::value_type(0.);
    ha::dtb::modulation::phase tmp_phase;
    tmp_phase.set_sample_rate(44100.f);
    tmp_phase.set_mode(ha::dtb::modulation::phase::modes::MODE_FREE);
    tmp_phase.set_rate(1);

    const auto offsetInSamplesRoundingErrors = 23;
    auto counter                             = 44100 + offsetInSamplesRoundingErrors;
    bool overflow                            = false;
    while (counter-- > 0)
        overflow = tmp_phase.update(current_phase, 1);

    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testTempoSyncedOverflowInOneStep)
{
    auto current_phase = ha::dtb::modulation::phase::value_type(0.);
    ha::dtb::modulation::phase tmp_phase;
    tmp_phase.set_sample_rate(44100.f);
    tmp_phase.set_mode(ha::dtb::modulation::phase::modes::MODE_TEMPO_SYNC);
    tmp_phase.set_tempo(120.f);
    tmp_phase.set_note_length(1.f);

    EXPECT_TRUE(tmp_phase.update(current_phase, 44100 * 2)); // 1 Note takes 2 seconds at 120BPM
    EXPECT_EQ(current_phase, 0.f);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testTempoSyncedOverflowInManyStep)
{
    auto current_phase = ha::dtb::modulation::phase::value_type(0.);
    ha::dtb::modulation::phase tmp_phase;
    tmp_phase.set_sample_rate(44100.f);
    tmp_phase.set_mode(ha::dtb::modulation::phase::modes::MODE_TEMPO_SYNC);
    tmp_phase.set_tempo(120.f);
    tmp_phase.set_note_length(1.f);

    const auto offsetInSamplesRoundingErrors = 37;
    auto counter                             = (44100 + offsetInSamplesRoundingErrors) * 2;
    bool overflow                            = false;
    while (counter-- > 0)
        overflow = tmp_phase.update(current_phase, 1);

    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testProjectSyncedOverflowInOneStep)
{
    auto current_phase = ha::dtb::modulation::phase::value_type(0.);
    ha::dtb::modulation::phase tmp_phase;
    tmp_phase.set_sample_rate(44100.f);
    tmp_phase.set_mode(ha::dtb::modulation::phase::modes::MODE_PROJECT_SYNC);
    tmp_phase.set_tempo(120.f);
    tmp_phase.set_note_length(1.f);

    tmp_phase.set_project_time(4.f);
    bool overflow = tmp_phase.update(current_phase, 1);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(current_phase, 0.f);
}

//------------------------------------------------------------------------
TEST(ModulationPhaseTest, testProjectSyncedTwoOverflowInOneStep)
{
    auto current_phase = ha::dtb::modulation::phase::value_type(0.);
    ha::dtb::modulation::phase tmp_phase;
    tmp_phase.set_sample_rate(44100.f);
    tmp_phase.set_mode(ha::dtb::modulation::phase::modes::MODE_PROJECT_SYNC);
    tmp_phase.set_tempo(120.f);
    tmp_phase.set_note_length(1.f);

    tmp_phase.set_project_time(8.f);
    bool overflow = tmp_phase.update(current_phase, 1);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(current_phase, 0.f);
}