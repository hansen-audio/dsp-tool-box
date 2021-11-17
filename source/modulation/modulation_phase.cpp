// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/modulation/modulation_phase.h"
#include <cassert>
#include <math.h>

namespace ha::dtb::modulation {

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
        phase_value = real(fmod(phase_value, phase_max));

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
real floor_by_cast(real value)
{
    return static_cast<real>(static_cast<i32>(value));
}

//-----------------------------------------------------------------------------
real normalise_phase(real value)
{
    return value - floor_by_cast(value);
}

//-----------------------------------------------------------------------------
void update_project_sync(mut_real& phase,
                         real const project_time,
                         real const rate)
{
    phase = project_time * rate;
    phase = normalise_phase(phase);
}

//-----------------------------------------------------------------------------
} // namespace

//-----------------------------------------------------------------------------
//  phase
//-----------------------------------------------------------------------------
bool PhaseImpl::advance(Phase const& self, mut_real& phase, i32 num_samples)
{
    switch (self.mode)
    {
        case Phase::SyncMode::Free:
            update_free_running(phase, num_samples, self.free_running_factor);
            break;
        case Phase::SyncMode::TempoSync:
            update_tempo_synced(phase, num_samples, self.tempo_synced_factor);
            break;
        case Phase::SyncMode::ProjectSync: {
            real old_phase = phase;
            update_project_sync(phase, self.project_time, self.rate);
            bool did_overflow = phase < old_phase;
            return did_overflow;
        }
        default:
            assert(!"Invalid mode");
            break;
    }

    return check_overflow(phase, PHASE_MAX);
}

//-----------------------------------------------------------------------------
bool PhaseImpl::advance_one_shot(Phase const& self,
                                 mut_real& value,
                                 i32 num_samples)
{
    if (value >= real(1.))
        return true;

    bool const is_overflow = advance(self, value, num_samples);
    if (is_overflow)
        value = real(1.);

    return is_overflow;
}

//-----------------------------------------------------------------------------
void PhaseImpl::set_sample_rate(Phase& self, real value)
{
    self.sample_rate_recip = real(1.) / value;
    self.free_running_factor =
        compute_free_running_factor(self.rate, self.sample_rate_recip);
    self.tempo_synced_factor =
        self.free_running_factor *
        compute_tempo_synced_factor(RECIPROCAL_60_SECONDS, self.tempo);
}

//-----------------------------------------------------------------------------
void PhaseImpl::set_rate(Phase& self, real value)
{
    self.rate = value;
    self.free_running_factor =
        compute_free_running_factor(self.rate, self.sample_rate_recip);
    self.tempo_synced_factor =
        self.free_running_factor *
        compute_tempo_synced_factor(RECIPROCAL_60_SECONDS, self.tempo);
}

//-----------------------------------------------------------------------------
void PhaseImpl::set_tempo(Phase& self, real value)
{
    self.tempo = value;
    self.tempo_synced_factor =
        self.free_running_factor *
        compute_tempo_synced_factor(RECIPROCAL_60_SECONDS, self.tempo);
}

//-----------------------------------------------------------------------------
void PhaseImpl::set_project_time(Phase& self, real value)
{
    self.project_time = value;
}

//-----------------------------------------------------------------------------
void PhaseImpl::set_note_len(Phase& self, real value)
{
    self.note_len   = value;
    real const rate = note_length_to_rate(value);
    set_rate(self, rate);
}

//-----------------------------------------------------------------------------
real PhaseImpl::note_length_to_rate(real value)
{
    assert(value > real(0.));
    return (real(1.) / value) * RECIPROCAL_BEATS_IN_NOTE;
}

//-----------------------------------------------------------------------------
void PhaseImpl::set_sync_mode(Phase& self, Phase::SyncMode value)
{
    self.mode = value;
}

//-----------------------------------------------------------------------------
Phase PhaseImpl::create()
{
    constexpr real INIT_NOTE_LEN = real(1. / 32.);
    Phase self;

    self.sample_rate_recip = real(1. / 44100.);
    self.mode              = Phase::SyncMode::TempoSync;
    self.tempo             = real(120.);

    PhaseImpl::set_note_len(self, INIT_NOTE_LEN);

    return self;
}

//-----------------------------------------------------------------------------
} // namespace ha::dtb::modulation