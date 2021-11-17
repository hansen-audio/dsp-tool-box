// Copyright(c) 2021 Hansen Audio.

#include "ha/dsp_tool_box/filtering/one_pole.h"

#include <math.h>

namespace ha::dtb::filtering {
namespace {

//-----------------------------------------------------------------------------
bool is_equal(const OnePole& self, real in)
{
    return in == self.z;
}

//-----------------------------------------------------------------------------
} // namespace

//-----------------------------------------------------------------------------
OnePole OnePoleImpl::create(real a)
{
    OnePole op{a, real(1.) - a, real(0.)};
    return op;
}

//-----------------------------------------------------------------------------
void OnePoleImpl::update_pole(OnePole& self, real a)
{
    self.a = a;
    self.b = real(1.) - self.a;
}

//-----------------------------------------------------------------------------
real OnePoleImpl::process(OnePole& self, real in)
{
    if (is_equal(self, in))
        return self.z;

    self.z = (in * self.b) + (self.z * self.a);
    return self.z;
}

//-----------------------------------------------------------------------------
void OnePoleImpl::reset(OnePole& self, real in)
{
    self.z = in;
}

//-----------------------------------------------------------------------------
real OnePoleImpl::tau_to_pole(real tau, real sample_rate)
{
    //! https://en.wikipedia.org/wiki/Time_constant
    //! (5 * Tau) means 99.3% reached thats sufficient.
    constexpr real RECIPROCAL_5 = real(1. / 5.);
    return real(exp(real(-1) / ((tau * RECIPROCAL_5) * sample_rate)));
}

//-----------------------------------------------------------------------------
} // namespace ha::dtb::filtering