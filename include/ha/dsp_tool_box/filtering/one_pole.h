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
class one_pole_filter final
{
public:
    //-------------------------------------------------------------------------
    struct context
    {
        mut_real a = 0;
        mut_real b = 0;
        mut_real z = 0;
    };

    static context create(real a = 0.9)
    {
        context cx{a, real(1.) - a, 0};

        return cx;
    }

    static void update_pole(context& cx, real a)
    {
        cx.a = a;
        cx.b = real(1.) - cx.a;
    }

    static real process(context& cx, real in)
    {
        if (is_equal(cx, in))
            return cx.z;

        cx.z = (in * cx.b) + (cx.z * cx.a);
        return cx.z;
    }

    static void reset(context& cx, real in) { cx.z = in; }

    static real tau_to_pole(real tau, real sample_rate)
    {
        //! https://en.wikipedia.org/wiki/Time_constant
        //! (5 * Tau) means 99.3% reached thats sufficient.
        constexpr real RECIPROCAL_5 = real(1. / 5.);
        return exp(real(-1) / ((tau * RECIPROCAL_5) * sample_rate));
    }
    //-------------------------------------------------------------------------
private:
    static bool is_equal(const context& cx, real in) { return in == cx.z; }
};

//-----------------------------------------------------------------------------
} // namespace filtering
} // namespace dtb
} // namespace ha