// Copyright(c) 2021 Hansen Audio.

#pragma once

#include <cassert>
#include <cstdint>
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
    using i32        = int32_t;

    static constexpr value_type RECIPROCAL_60_SECONDS    = value_type(1.) / value_type(60.);
    static constexpr value_type RECIPROCAL_BEATS_IN_NOTE = value_type(1.) / value_type(4.);
    static constexpr value_type PHASE_MAX                = value_type(1.);

    enum modes
    {
        MODE_FREE = 0,
        MODE_TEMPO_SYNC,
        MODE_PROJECT_SYNC
    };

    using mode = i32;

    bool update(i32 num_samples);

    value_type get_phase() const { return current_phase; }

    void set_mode(mode value);

    void set_tempo(value_type value);

    void set_rate(value_type value);

    void set_sample_rate(value_type value);

    void set_project_time(value_type value);

    void reset(value_type init = value_type(0.));

    void set_note_length(value_type value);

    static value_type note_length_to_rate(value_type length);

    //--------------------------------------------------------------------
private:
    inline bool check_overflow(value_type& phase);

    inline void update_free_running(i32 num_samples);

    inline void update_tempo_synced(i32 num_samples);

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
    i32 current_mode             = MODE_FREE;

    value_type free_running_factor = value_type(0.);
    value_type tempo_synced_factor = value_type(0.);
};

//------------------------------------------------------------------------
class one_shot_phase : public phase
{
public:
    //--------------------------------------------------------------------
    one_shot_phase() = default;

    bool update_one_shot(i32 num_samples);

    value_type get_one_shot_phase() const;

    void reset_one_shot(value_type init = value_type(0.));

    bool is_one_shot_overflow() const;

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
