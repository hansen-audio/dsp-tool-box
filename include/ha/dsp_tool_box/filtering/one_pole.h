// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/dsp_tool_box/core/types.h"
#include <math.h>

namespace ha {
namespace dtb {
namespace filtering {

//------------------------------------------------------------------------
/** A one-pole filter for e.g. parameter smoothing. */
//------------------------------------------------------------------------
class one_pole_filter final
{
public:
    //--------------------------------------------------------------------
    struct context_data
    {
        mut_real a = 0;
        mut_real b = 0;
        mut_real z = 0;
    };

    static context_data init(real a = 0.9)
    {
        context_data data{a, real(1.) - a, 0};

        return data;
    }

    static void update_pole(real a, context_data& data)
    {
        data.a = a;
        data.b = real(1.) - data.a;
    }

    static real process(real in, context_data& data)
    {
        if (is_equal(in, data))
            return data.z;

        data.z = (in * data.b) + (data.z * data.a);
        return data.z;
    }

    static void reset(real in, context_data& data) { data.z = in; }

    static real tau_to_pole(real tau, real sample_rate)
    {
        //! https://en.wikipedia.org/wiki/Time_constant
        //! (5 * Tau) means 99.3% reached thats sufficient.
        constexpr real RECIPROCAL_5 = real(1. / 5.);
        return exp(real(-1) / ((tau * RECIPROCAL_5) * sample_rate));
    }
    //--------------------------------------------------------------------
private:
    static bool is_equal(real in, const context_data& data) { return in == data.z; }
};

//-----------------------------------------------------------------------------
} // namespace filtering
} // namespace dtb
} // namespace ha