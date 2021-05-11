// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/dsp_tool_box/core/types.h"
#include <math.h>

namespace ha {
namespace dtb {
namespace filtering {

/**
 * @brief A one-pole filter for e.g. parameter smoothing.
 *
 */
struct one_pole_filter final
{
    struct context
    {
        mut_real a = 0;
        mut_real b = 0;
        mut_real z = 0;
    };

    static context create(real a = 0.9);
    static void update_pole(context& cx, real a);
    static real process(context& cx, real in);
    static void reset(context& cx, real in);
    static real tau_to_pole(real tau, real sample_rate);
};

//-----------------------------------------------------------------------------
} // namespace filtering
} // namespace dtb
} // namespace ha