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
    enum modes
    {
        MODE_FREE = 0,
        MODE_TEMPO_SYNC,
        MODE_PROJECT_SYNC
    };

    using mode = mut_i32;

    struct context
    {
        mut_real rate                = real(0.);
        mut_real tempo               = real(120.);
        mut_real sample_rate_recip   = real(1.);
        mut_real project_time        = real(0.);
        mode current_mode            = MODE_FREE;
        mut_real free_running_factor = real(0.);
        mut_real tempo_synced_factor = real(0.);
    };

    static bool advance_one_shot(context& context, mut_real& value, i32 num_samples);
    static bool advance(context const& context, mut_real& value, i32 num_samples);
    static void set_mode(context& context, mode value);
    static void set_tempo(context& context, real value);
    static void set_rate(context& context, real value);
    static void set_sample_rate(context& context, real value);
    static void set_project_time(context& context, real value);
    static void set_note_length(context& context, real value);
    static real note_length_to_rate(real value);
};

//------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha
