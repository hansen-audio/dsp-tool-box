// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/dsp_tool_box/core/types.h"

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
        Real a = 0;
        Real b = 0;
        Real z = 0;
    };

    static context_data init(Real a = 0.9)
    {
        context_data data{a, Real(1.) - a, 0};

        return data;
    }

    static void update_pole(Real a, context_data& data)
    {
        data.a = a;
        data.b = Real(1.) - data.a;
    }

    static Real smooth(Real in, context_data& data)
    {
        if (is_equal(in, data))
            return data.z;

        data.z = (in * data.b) + (data.z * data.a);
        return data.z;
    }

    static void reset(Real in, context_data& data) { data.z = in; }

    static Real tau_to_pole(Real tau, Real sample_rate)
    {
        //! https://en.wikipedia.org/wiki/Time_constant
        //! (5 * Tau) means 99.3% reached thats sufficient.
        constexpr Real RECIPROCAL_5 = Real(1. / 5.);
        return exp(Real(-1) / ((tau * RECIPROCAL_5) * sample_rate));
    }
    //--------------------------------------------------------------------
private:
    static bool is_equal(Real in, const context_data& data) { return in == data.z; }
};

//-----------------------------------------------------------------------------
} // namespace filtering
} // namespace dtb
} // namespace ha