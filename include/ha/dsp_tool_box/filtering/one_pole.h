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
        float_t a = 0;
        float_t b = 0;
        float_t z = 0;
    };

    static context_data init(float_t a = 0.9)
    {
        context_data data{a, float_t(1.) - a, 0};

        return data;
    }

    static void update_pole(float_t a, context_data& data)
    {
        data.a = a;
        data.b = float_t(1.) - data.a;
    }

    static float_t process(float_t in, context_data& data)
    {
        if (is_equal(in, data))
            return data.z;

        data.z = (in * data.b) + (data.z * data.a);
        return data.z;
    }

    static void reset(float_t in, context_data& data) { data.z = in; }

    static float_t tau_to_pole(float_t tau, float_t sample_rate)
    {
        //! https://en.wikipedia.org/wiki/Time_constant
        //! (5 * Tau) means 99.3% reached thats sufficient.
        constexpr float_t RECIPROCAL_5 = float_t(1. / 5.);
        return exp(float_t(-1) / ((tau * RECIPROCAL_5) * sample_rate));
    }
    //--------------------------------------------------------------------
private:
    static bool is_equal(float_t in, const context_data& data) { return in == data.z; }
};

//-----------------------------------------------------------------------------
} // namespace filtering
} // namespace dtb
} // namespace ha