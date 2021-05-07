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
    //--------------------------------------------------------------------
    struct context
    {
        mut_real a = 0;
        mut_real b = 0;
        mut_real z = 0;
    };

    static context create(real a = 0.9)
    {
        context ctx{a, real(1.) - a, 0};

        return ctx;
    }

    static void update_pole(context& ctx, real a)
    {
        ctx.a = a;
        ctx.b = real(1.) - ctx.a;
    }

    static real process(context& ctx, real in)
    {
        if (is_equal(ctx, in))
            return ctx.z;

        ctx.z = (in * ctx.b) + (ctx.z * ctx.a);
        return ctx.z;
    }

    static void reset(context& ctx, real in) { ctx.z = in; }

    static real tau_to_pole(real tau, real sample_rate)
    {
        //! https://en.wikipedia.org/wiki/Time_constant
        //! (5 * Tau) means 99.3% reached thats sufficient.
        constexpr real RECIPROCAL_5 = real(1. / 5.);
        return exp(real(-1) / ((tau * RECIPROCAL_5) * sample_rate));
    }
    //--------------------------------------------------------------------
private:
    static bool is_equal(const context& ctx, real in) { return in == ctx.z; }
};

//-----------------------------------------------------------------------------
} // namespace filtering
} // namespace dtb
} // namespace ha