// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/filtering/one_pole.h"

#include <math.h>

namespace ha::dtb::filtering {
namespace {

//-----------------------------------------------------------------------------
bool is_equal(const one_pole_filter::context& cx, real in)
{
    return in == cx.z;
}

//-----------------------------------------------------------------------------
} // namespace

//-----------------------------------------------------------------------------
one_pole_filter::context one_pole_filter::create(real a)
{
    context cx{a, real(1.) - a, real(0.)};

    return cx;
}

//-----------------------------------------------------------------------------
void one_pole_filter::update_pole(context& cx, real a)
{
    cx.a = a;
    cx.b = real(1.) - cx.a;
}

//-----------------------------------------------------------------------------
real one_pole_filter::process(context& cx, real in)
{
    if (is_equal(cx, in))
        return cx.z;

    cx.z = (in * cx.b) + (cx.z * cx.a);
    return cx.z;
}

//-----------------------------------------------------------------------------
void one_pole_filter::reset(context& cx, real in)
{
    cx.z = in;
}

//-----------------------------------------------------------------------------
real one_pole_filter::tau_to_pole(real tau, real sample_rate)
{
    //! https://en.wikipedia.org/wiki/Time_constant
    //! (5 * Tau) means 99.3% reached thats sufficient.
    constexpr real RECIPROCAL_5 = real(1. / 5.);
    return real(exp(real(-1) / ((tau * RECIPROCAL_5) * sample_rate)));
}

//-----------------------------------------------------------------------------
} // namespace ha::dtb::filtering