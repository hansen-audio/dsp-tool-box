// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/modulation/modulation_phase.h"

namespace ha {
namespace dtb {
namespace modulation {

// -----------------------------------------------------------------------
//  phase
//------------------------------------------------------------------------
bool phase::check_overflow(value_type& phase_value)
{
    bool overflow = phase_value >= PHASE_MAX;
    if (overflow)
        phase_value = fmodf(phase_value, PHASE_MAX);

    return overflow;
}

//------------------------------------------------------------------------
bool phase::update(i32 num_samples)
{
    switch (current_mode)
    {
        case MODE_FREE:
            update_free_running(num_samples);
            break;
        case MODE_TEMPO_SYNC:
            update_tempo_synced(num_samples);
            break;
        case MODE_PROJECT_SYNC:
            update_project_sync();
            break;
        default:
            assert(!"Invalid mode");
            break;
    }

    return check_overflow(current_phase);
}

//------------------------------------------------------------------------
phase::value_type phase::compute_free_running_factor(value_type rate,
                                                     value_type sample_rate_recip) const
{
    return rate * sample_rate_recip;
}

//------------------------------------------------------------------------
phase::value_type phase::compute_tempo_synced_factor(value_type sixty_seconds_recip,
                                                     value_type tempo) const
{
    return sixty_seconds_recip * tempo;
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
void phase::update_free_running(i32 num_samples)
{
    current_phase += static_cast<value_type>(num_samples) * free_running_factor;
}

//------------------------------------------------------------------------
void phase::update_tempo_synced(i32 num_samples)
{
    current_phase += static_cast<value_type>(num_samples) * tempo_synced_factor;
}

//------------------------------------------------------------------------
void phase::update_project_sync()
{
    current_phase = project_time * rate;
}

//------------------------------------------------------------------------
void phase::set_project_time(value_type value)
{
    project_time = value;
}

//------------------------------------------------------------------------
void phase::reset(value_type value)
{
    current_phase = value;
}

//------------------------------------------------------------------------
void phase::set_note_length(value_type value)
{
    auto rate = note_length_to_rate(value);
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
bool one_shot_phase::update_one_shot(i32 num_samples)
{
    if (did_overflow)
        return true;

    did_overflow = phase::update(num_samples);
    return did_overflow;
}

//------------------------------------------------------------------------
one_shot_phase::value_type one_shot_phase::get_one_shot_phase() const
{
    if (did_overflow)
        return value_type(1.);

    return phase::get_phase();
}

//------------------------------------------------------------------------
void one_shot_phase::reset_one_shot(value_type value)
{
    did_overflow = value < value_type(1.) ? false : true;
    phase::reset(value);
}

//------------------------------------------------------------------------
bool one_shot_phase::is_one_shot_overflow() const
{
    return did_overflow;
}

//------------------------------------------------------------------------

//------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha