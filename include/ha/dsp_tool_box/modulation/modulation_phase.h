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
    enum class sync_mode
    {
        FREE = 0,
        TEMPO_SYNC,
        PROJECT_SYNC
    };

    struct context
    {
        mut_real rate                = real(0.);
        mut_real tempo               = real(120.);
        mut_real sample_rate_recip   = real(1.);
        mut_real project_time        = real(0.);
        sync_mode mode               = sync_mode::FREE;
        mut_real free_running_factor = real(0.);
        mut_real tempo_synced_factor = real(0.);
    };

    static bool
    advance_one_shot(context const& cx, mut_real& value, i32 num_samples);
    static bool advance(context const& cx, mut_real& value, i32 num_samples);
    static void set_sync_mode(context& cx, sync_mode value);
    static void set_tempo(context& cx, real value);
    static void set_rate(context& cx, real value);
    static void set_sample_rate(context& cx, real value);
    static void set_project_time(context& cx, real value);
    static void set_note_length(context& cx, real value);
    static real note_length_to_rate(real value);
};

//------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha
