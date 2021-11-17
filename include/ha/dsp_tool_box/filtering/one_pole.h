// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/dsp_tool_box/core/types.h"
#include <math.h>

namespace ha::dtb::filtering {

/**
 * @brief A one-pole filter for e.g. parameter smoothing.
 *
 */
struct OnePole final
{
    mut_real a = 0;
    mut_real b = 0;
    mut_real z = 0;
};

struct OnePoleImpl final
{
    static OnePole create(real a = 0.9);
    static void update_pole(OnePole& self, real a);
    static real process(OnePole& self, real in);
    static void reset(OnePole& self, real in);
    static real tau_to_pole(real tau, real sample_rate);
};

//-----------------------------------------------------------------------------
} // namespace ha::dtb::filtering