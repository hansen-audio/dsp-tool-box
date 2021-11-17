// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/modulation/modulation_phase.h"
#include "gtest/gtest.h"

using real = ha::dtb::real;

using namespace ha::dtb::modulation;

/**
 * @brief modulation_phase_test
 */
TEST(modulation_phase_test, test_free_running_overflow_in_one_step)
{
    auto val = real(0.);
    Phase phase;
    PhaseImpl::set_sample_rate(phase, 44100.f);
    PhaseImpl::set_sync_mode(phase, Phase::SyncMode::Free);
    PhaseImpl::set_rate(phase, 1);
    PhaseImpl::advance(phase, val, 44100);

    bool const overflow = PhaseImpl::advance(phase, val, 44100);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(val, 0.f);
}

//------------------------------------------------------------------------
TEST(modulation_phase_test, test_free_running_overflow_in_many_step)
{
    auto val = real(0.);
    Phase phase;
    PhaseImpl::set_sample_rate(phase, 44100.f);
    PhaseImpl::set_sync_mode(phase, Phase::SyncMode::Free);
    PhaseImpl::set_rate(phase, 1);

    const auto offset_in_samples_rounding_errors = 23;
    auto counter  = 44100 + offset_in_samples_rounding_errors;
    bool overflow = false;
    while (counter-- > 0)
        overflow = PhaseImpl::advance(phase, val, 1);

    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(modulation_phase_test, test_tempo_synced_overflow_in_one_step)
{
    auto val   = real(0.);
    auto phase = PhaseImpl::create();
    PhaseImpl::set_note_len(phase, 1.f);

    // 1 Note takes 2 seconds at 120BPM
    bool const overflow = PhaseImpl::advance(phase, val, 44100 * 2);
    EXPECT_TRUE(overflow);
    EXPECT_EQ(val, 0.f);
}

//------------------------------------------------------------------------
TEST(modulation_phase_test, test_tempo_synced_overflow_in_many_step)
{
    auto val   = real(0.);
    auto phase = PhaseImpl::create();
    PhaseImpl::set_note_len(phase, 1.f);

    const auto offset_in_samples_rounding_errors = 37;
    auto counter  = (44100 + offset_in_samples_rounding_errors) * 2;
    bool overflow = false;
    while (counter-- > 0)
        overflow = PhaseImpl::advance(phase, val, 1);

    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(modulation_phase_test, test_project_synced_overflow)
{
    auto val   = real(0.);
    auto phase = PhaseImpl::create();
    PhaseImpl::set_sync_mode(phase, Phase::SyncMode::ProjectSync);
    PhaseImpl::set_note_len(phase, 1.f);
    PhaseImpl::set_project_time(phase, 3.9f);
    bool overflow = PhaseImpl::advance(phase, val, 1);
    EXPECT_FALSE(overflow);

    PhaseImpl::set_project_time(phase, 4.0f);
    overflow = PhaseImpl::advance(phase, val, 1);
    EXPECT_TRUE(overflow);
}

//------------------------------------------------------------------------
TEST(modulation_phase_test, test_floor_by_cast)
{
    using i32 = ha::dtb::i32;

    real val0               = 3.25;
    real val0_floor_by_cast = static_cast<real>(static_cast<i32>(val0));
    EXPECT_EQ(val0_floor_by_cast, 3.0);
}

//------------------------------------------------------------------------
TEST(modulation_phase_test, test_floor_by_cast_2)
{
    using i32 = ha::dtb::i32;

    real val0               = 3.75;
    real val0_floor_by_cast = static_cast<real>(static_cast<i32>(val0));
    EXPECT_EQ(val0_floor_by_cast, 3.0);
}