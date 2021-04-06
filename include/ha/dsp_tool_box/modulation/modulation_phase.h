// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/dsp_tool_box/core/types.h"
#include <cassert>
#include <math.h>

namespace ha {
namespace dtb {
namespace modulation {

//------------------------------------------------------------------------
/**
 * phase
 */
struct phase
{
    using value_type = float;
    enum modes
    {
        MODE_FREE = 0,
        MODE_TEMPO_SYNC,
        MODE_PROJECT_SYNC
    };

    using mode = i32;

    struct context_t
    {
        value_type rate              = value_type(0.);
        value_type tempo             = value_type(120.);
        value_type sample_rate_recip = value_type(1.);
        value_type project_time      = value_type(0.);
        i32 current_mode             = MODE_FREE;

        value_type free_running_factor = value_type(0.);
        value_type tempo_synced_factor = value_type(0.);
    };

    static bool update(context_t& context, value_type& phase, i32 num_samples);
    static void set_mode(context_t& context, mode value);
    static void set_tempo(context_t& context, value_type value);
    static void set_rate(context_t& context, value_type value);
    static void set_sample_rate(context_t& context, value_type value);
    static void set_project_time(context_t& context, value_type value);
    static void set_note_length(context_t& context, value_type value);
    static value_type note_length_to_rate(value_type length);
};

/**
 * one_shot_phase
 */
struct one_shot_phase
{
    struct context_t : public phase::context_t
    {
        bool did_overflow = false;
    };

    static bool update_one_shot(context_t& context, phase::value_type& phase, i32 num_samples);
    static bool is_one_shot_overflow(context_t& context, phase::value_type phase);
};

//------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha
