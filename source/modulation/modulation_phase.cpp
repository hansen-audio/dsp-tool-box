// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/modulation/modulation_phase.h"

namespace ha {
namespace dtb {
namespace modulation {

//------------------------------------------------------------------------
static constexpr float_t RECIPROCAL_60_SECONDS    = float_t(1.) / float_t(60.);
static constexpr float_t RECIPROCAL_BEATS_IN_NOTE = float_t(1.) / float_t(4.);
static constexpr float_t PHASE_MAX                = float_t(1.);

namespace {
//------------------------------------------------------------------------
bool check_overflow(float_t& phase_value, float const phase_max)
{
    bool const overflow = phase_value >= phase_max;
    if (overflow)
        phase_value = fmodf(phase_value, phase_max);

    return overflow;
}

//------------------------------------------------------------------------
float_t compute_free_running_factor(float_t const rate, float_t const sample_rate_recip)
{
    return rate * sample_rate_recip;
}

//------------------------------------------------------------------------
float_t compute_tempo_synced_factor(float_t const sixty_seconds_recip, float_t const tempo)
{
    return sixty_seconds_recip * tempo;
}

//------------------------------------------------------------------------
void update_free_running(float_t& phase, i32 const num_samples, float_t const free_running_factor)
{
    phase += static_cast<float_t>(num_samples) * free_running_factor;
}

//------------------------------------------------------------------------
void update_tempo_synced(float_t& phase, i32 const num_samples, float_t const tempo_synced_factor)
{
    phase += static_cast<float_t>(num_samples) * tempo_synced_factor;
}

//------------------------------------------------------------------------
void update_project_sync(float_t& phase, float_t const project_time, float_t const rate)
{
    phase = project_time * rate;
}

//------------------------------------------------------------------------
} // namespace

//------------------------------------------------------------------------
//  phase
//------------------------------------------------------------------------
bool phase::update(value_type& phase, i32 num_samples)
{
    switch (current_mode)
    {
        case MODE_FREE:
            update_free_running(phase, num_samples, free_running_factor);
            break;
        case MODE_TEMPO_SYNC:
            update_tempo_synced(phase, num_samples, tempo_synced_factor);
            break;
        case MODE_PROJECT_SYNC:
            update_project_sync(phase, project_time, rate);
            break;
        default:
            assert(!"Invalid mode");
            break;
    }

    return check_overflow(phase, PHASE_MAX);
}

//------------------------------------------------------------------------
void phase::set_sample_rate(value_type value)
{
    sample_rate_recip   = value_type(1.) / value;
    free_running_factor = compute_free_running_factor(rate, sample_rate_recip);
    tempo_synced_factor =
        free_running_factor * compute_tempo_synced_factor(RECIPROCAL_60_SECONDS, tempo);
}

//------------------------------------------------------------------------
void phase::set_rate(value_type value)
{
    rate                = value;
    free_running_factor = compute_free_running_factor(rate, sample_rate_recip);
    tempo_synced_factor =
        free_running_factor * compute_tempo_synced_factor(RECIPROCAL_60_SECONDS, tempo);
}

//------------------------------------------------------------------------
void phase::set_tempo(value_type value)
{
    tempo = value;
    tempo_synced_factor =
        free_running_factor * compute_tempo_synced_factor(RECIPROCAL_60_SECONDS, tempo);
}

//------------------------------------------------------------------------
void phase::set_project_time(value_type value)
{
    project_time = value;
}

//------------------------------------------------------------------------
void phase::set_note_length(value_type value)
{
    value_type const rate = note_length_to_rate(value);
    set_rate(rate);
}

//------------------------------------------------------------------------
phase::value_type phase::note_length_to_rate(value_type length)
{
    assert(length > value_type(0.));
    return (value_type(1.) / length) * RECIPROCAL_BEATS_IN_NOTE;
}

//------------------------------------------------------------------------
void phase::set_mode(mode value)
{
    current_mode = value;
}

//------------------------------------------------------------------------
//  one_shot_phase
//------------------------------------------------------------------------
bool one_shot_phase::update_one_shot(value_type& phase, i32 num_samples)
{
    if (did_overflow)
        return true;

    did_overflow = phase::update(phase, num_samples);
    return did_overflow;
}

//------------------------------------------------------------------------
bool one_shot_phase::is_one_shot_overflow(value_type phase) const
{
    return phase > value_type(1.);
}

//------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha