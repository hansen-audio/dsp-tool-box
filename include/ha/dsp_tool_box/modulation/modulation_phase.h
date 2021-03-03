// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/dsp_tool_box/core/types.h"
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

    enum modes
    {
        MODE_FREE = 0,
        MODE_TEMPO_SYNC,
        MODE_PROJECT_SYNC
    };

    using mode = i32;

    bool update(value_type& phase, i32 num_samples);
    void set_mode(mode value);
    void set_tempo(value_type value);
    void set_rate(value_type value);
    void set_sample_rate(value_type value);
    void set_project_time(value_type value);
    void set_note_length(value_type value);

    static value_type note_length_to_rate(value_type length);

    //--------------------------------------------------------------------
private:
    value_type rate              = value_type(0.);
    value_type tempo             = value_type(120.);
    value_type sample_rate_recip = value_type(1.);
    value_type project_time      = value_type(0.);
    i32 current_mode             = MODE_FREE;

    value_type free_running_factor = value_type(0.);
    value_type tempo_synced_factor = value_type(0.);
};

//------------------------------------------------------------------------
// one_shot_phase
//------------------------------------------------------------------------
class one_shot_phase : public phase
{
public:
    //--------------------------------------------------------------------
    one_shot_phase() = default;

    bool update_one_shot(value_type& phase, i32 num_samples);
    bool is_one_shot_overflow(value_type phase) const;

    //--------------------------------------------------------------------
private:
    bool did_overflow = false;
};
//------------------------------------------------------------------------
// #include "modulation_phase.inl.h"
//------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha
