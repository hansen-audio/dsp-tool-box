// Copyright(c) 2021 Hansen Audio.

#pragma once

#include <cassert>
#include <math.h>

namespace ha {
namespace dtb {
namespace modulation {

//------------------------------------------------------------------------
// phase
//------------------------------------------------------------------------
class phase
{
public:
    //--------------------------------------------------------------------
    phase() = default;

    using value_type = float;

    static constexpr phase::value_type creciprocal_60_seconds    = value_type(1.) / value_type(60.);
    static constexpr phase::value_type creciprocal_beats_in_note = value_type(1.) / value_type(4.);

    enum eMode
    {
        kModeFree = 0,
        kModeTempoSync,
        kModeProjectSync
    };

    inline bool update(int num_samples);
    value_type get_phase() const { return current_phase; }

    void set_mode(eMode mode) { mMode = mode; }

    inline void set_tempo(value_type value);
    inline void set_rate(value_type value);
    inline void set_sample_rate(value_type value);

    void set_project_time(value_type value) { project_time = value; }

    void reset(value_type init = value_type(0.)) { current_phase = init; }

    void set_note_length(value_type value)
    {
        auto rate = note_length_to_rate(value);
        set_rate(rate);
    }

    static value_type note_length_to_rate(value_type length)
    {
        assert(length > 0.f);
        return (1.f / length) * creciprocal_beats_in_note;
    }

    //--------------------------------------------------------------------
private:
    inline bool check_overflow(value_type& phase);
    inline void update_free_running(int num_samples);
    inline void update_tempo_synced(int num_samples);
    inline void update_project_sync();

    inline value_type compute_free_running_factor(value_type rate,
                                                  value_type sample_rate_recip) const;
    inline value_type compute_tempo_synced_factor(value_type rate,
                                                  value_type sample_rate_recip) const;

    value_type current_phase     = value_type(0.);
    value_type rate              = value_type(0.);
    value_type tempo             = value_type(120.);
    value_type sample_rate_recip = value_type(1.);
    value_type project_time      = value_type(0.);
    int mMode                    = kModeFree;

    value_type free_running_factor = value_type(0.);
    value_type tempo_synced_factor = value_type(0.);
};

//------------------------------------------------------------------------
class one_shot_phase : public phase
{
public:
    //--------------------------------------------------------------------
    one_shot_phase() = default;

    bool update_one_shot(int num_samples)
    {
        if (did_overflow)
            return true;

        did_overflow = phase::update(num_samples);
        return did_overflow;
    }

    value_type get_one_shot_phase() const
    {
        if (did_overflow)
            return value_type(1.);

        return phase::get_phase();
    }

    void reset_one_shot(value_type init = 0.f)
    {
        did_overflow = init < value_type(1.) ? false : true;
        phase::reset(init);
    }

    bool is_one_shot_overflow() const { return did_overflow; }

    //--------------------------------------------------------------------
private:
    bool did_overflow = false;
};
//------------------------------------------------------------------------
#include "modulation_phase.inl.h"
//------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha
