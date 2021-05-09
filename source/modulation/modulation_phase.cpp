// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/modulation/modulation_phase.h"
#include <cassert>
#include <math.h>

namespace ha {
namespace dtb {
namespace modulation {

//-----------------------------------------------------------------------------
static constexpr real RECIPROCAL_60_SECONDS    = real(1.) / real(60.);
static constexpr real RECIPROCAL_BEATS_IN_NOTE = real(1.) / real(4.);
static constexpr real PHASE_MAX                = real(1.);

namespace {
//-----------------------------------------------------------------------------
bool check_overflow(mut_real& phase_value, real const phase_max)
{
    bool const overflow = phase_value >= phase_max;
    if (overflow)
        phase_value = fmodf(phase_value, phase_max);

    return overflow;
}

//-----------------------------------------------------------------------------
real compute_free_running_factor(real const rate, real const sample_rate_recip)
{
    return rate * sample_rate_recip;
}

//-----------------------------------------------------------------------------
real compute_tempo_synced_factor(real const sixty_seconds_recip,
                                 real const tempo)
{
    return sixty_seconds_recip * tempo;
}

//-----------------------------------------------------------------------------
void update_free_running(mut_real& phase,
                         i32 num_samples,
                         real const free_running_factor)
{
    phase += static_cast<real>(num_samples) * free_running_factor;
}

//-----------------------------------------------------------------------------
void update_tempo_synced(mut_real& phase,
                         i32 num_samples,
                         real const tempo_synced_factor)
{
    phase += static_cast<real>(num_samples) * tempo_synced_factor;
}

//-----------------------------------------------------------------------------
void update_project_sync(mut_real& phase,
                         real const project_time,
                         real const rate)
{
    phase = project_time * rate;
}

//-----------------------------------------------------------------------------
} // namespace

//-----------------------------------------------------------------------------
//  phase
//-----------------------------------------------------------------------------
bool phase::advance(context const& cx, mut_real& phase, i32 num_samples)
{
    switch (cx.mode)
    {
        case sync_mode::FREE:
            update_free_running(phase, num_samples, cx.free_running_factor);
            break;
        case sync_mode::TEMPO_SYNC:
            update_tempo_synced(phase, num_samples, cx.tempo_synced_factor);
            break;
        case sync_mode::PROJECT_SYNC:
            update_project_sync(phase, cx.project_time, cx.rate);
            break;
        default:
            assert(!"Invalid mode");
            break;
    }

    return check_overflow(phase, PHASE_MAX);
}

//-----------------------------------------------------------------------------
bool phase::advance_one_shot(context const& cx,
                             mut_real& value,
                             i32 num_samples)
{
    if (value >= real(1.))
        return true;

    bool const is_overflow = advance(cx, value, num_samples);
    if (is_overflow)
        value = real(1.);

    return is_overflow;
}

//-----------------------------------------------------------------------------
void phase::set_sample_rate(context& cx, real value)
{
    cx.sample_rate_recip = real(1.) / value;
    cx.free_running_factor =
        compute_free_running_factor(cx.rate, cx.sample_rate_recip);
    cx.tempo_synced_factor =
        cx.free_running_factor *
        compute_tempo_synced_factor(RECIPROCAL_60_SECONDS, cx.tempo);
}

//-----------------------------------------------------------------------------
void phase::set_rate(context& cx, real value)
{
    cx.rate = value;
    cx.free_running_factor =
        compute_free_running_factor(cx.rate, cx.sample_rate_recip);
    cx.tempo_synced_factor =
        cx.free_running_factor *
        compute_tempo_synced_factor(RECIPROCAL_60_SECONDS, cx.tempo);
}

//-----------------------------------------------------------------------------
void phase::set_tempo(context& cx, real value)
{
    cx.tempo = value;
    cx.tempo_synced_factor =
        cx.free_running_factor *
        compute_tempo_synced_factor(RECIPROCAL_60_SECONDS, cx.tempo);
}

//-----------------------------------------------------------------------------
void phase::set_project_time(context& cx, real value)
{
    cx.project_time = value;
}

//-----------------------------------------------------------------------------
void phase::set_note_length(context& cx, real value)
{
    real const rate = note_length_to_rate(value);
    set_rate(cx, rate);
}

//-----------------------------------------------------------------------------
real phase::note_length_to_rate(real value)
{
    assert(value > real(0.));
    return (real(1.) / value) * RECIPROCAL_BEATS_IN_NOTE;
}

//-----------------------------------------------------------------------------
void phase::set_sync_mode(context& cx, sync_mode value)
{
    cx.mode = value;
}

//-----------------------------------------------------------------------------
phase::context phase::create()
{
    context cx;

    cx.sample_rate_recip = real(1. / 44100.);
    cx.mode              = sync_mode::TEMPO_SYNC;
    cx.tempo             = real(120.);

    phase::set_note_length(cx, real(1. / 32.));

    return cx;
}

//-----------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha