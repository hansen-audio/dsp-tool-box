// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/modulation/modulation_phase.h"
#include "gtest/gtest.h"

using real = ha::dtb::real;

/**
 * @brief modulation_phase_test
 */
TEST(modulation_phase_test, test_free_running_overflow_in_one_step)
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
TEST(modulation_phase_test, test_free_running_overflow_in_many_step)
{
    using phase      = ha::dtb::modulation::phase;
    auto phase_value = real(0.);
    phase::context cx;
    phase::set_sample_rate(cx, 44100.f);
    phase::set_sync_mode(cx, phase::sync_mode::FREE);
    phase::set_rate(cx, 1);

    const auto offset_in_samples_rounding_errors = 23;
    auto counter  = 44100 + offset_in_samples_rounding_errors;
    bool overflow = false;
    while (counter-- > 0)
        overflow = phase::advance(cx, phase_value, 1);

    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(modulation_phase_test, test_tempo_synced_overflow_in_one_step)
{
    using phase       = ha::dtb::modulation::phase;
    auto phase_value  = real(0.);
    phase::context cx = phase::create();
    phase::set_note_len(cx, 1.f);

    // 1 Note takes 2 seconds at 120BPM
    bool const overflow = phase::advance(cx, phase_value, 44100 * 2);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(phase_value, 0.f);
}

//------------------------------------------------------------------------
TEST(modulation_phase_test, test_tempo_synced_overflow_in_many_step)
{
    using phase       = ha::dtb::modulation::phase;
    auto phase_value  = real(0.);
    phase::context cx = phase::create();
    phase::set_note_len(cx, 1.f);

    const auto offset_in_samples_rounding_errors = 37;
    auto counter  = (44100 + offset_in_samples_rounding_errors) * 2;
    bool overflow = false;
    while (counter-- > 0)
        overflow = phase::advance(cx, phase_value, 1);

    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(modulation_phase_test, test_project_synced_overflow_in_one_step)
{
    using phase       = ha::dtb::modulation::phase;
    auto phase_value  = real(0.);
    phase::context cx = phase::create();
    phase::set_sync_mode(cx, phase::sync_mode::PROJECT_SYNC);
    phase::set_note_len(cx, 1.f);
    phase::set_project_time(cx, 4.f);

    bool overflow = phase::advance(cx, phase_value, 1);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(phase_value, 0.f);
}

//------------------------------------------------------------------------
TEST(modulation_phase_test, test_project_synced_two_overflow_in_one_step)
{
    using phase       = ha::dtb::modulation::phase;
    auto phase_value  = real(0.);
    phase::context cx = phase::create();
    phase::set_sync_mode(cx, phase::sync_mode::PROJECT_SYNC);
    phase::set_note_len(cx, 1.f);
    phase::set_project_time(cx, 8.f);

    bool overflow = phase::advance(cx, phase_value, 1);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(phase_value, 0.f);
}