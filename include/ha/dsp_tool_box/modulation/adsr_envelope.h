// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/dsp_tool_box/core/types.h"
#include <math.h>
#include <utility>

namespace ha {
namespace dtb {
namespace modulation {
namespace easing {

//------------------------------------------------------------------------
template <typename T>
T ease_virus_ti(const T value)
{
    /*
        float x = powf (0.5f, x * 14.f);
        float db = 20.f * log10f (x);
        --> ca. -96dB....sufficient!
    */
    constexpr T DB_VALUE      = T(96.);
    constexpr T FACTOR        = pow(T(10.), DB_VALUE / T(20.));
    constexpr T MULTIPLICATOR = log(FACTOR) / log(T(2.));
    return T(1.) - pow(T(0.5), value * MULTIPLICATOR);
}

//------------------------------------------------------------------------
} // namespace easing

//-----------------------------------------------------------------------------
class adsr_envelope
{
public:
    //-------------------------------------------------------------------------
    adsr_envelope() = default;

    //! Value: normalized, normalizedRecip
    using value = std::pair<mut_real, mut_real>;

    static constexpr real MIN_VALUE = 0.;
    static constexpr real MAX_VALUE = 1.;

    enum class stages
    {
        STAGE_BEFORE_TRIGGER,
        STAGE_ATTACK,
        STAGE_DECAY,
        STAGE_SUSTAIN,
        STAGE_RELEASE
    };

    struct context
    {
        stages stage;
        mut_real time_seconds;
        mut_real release_value;
    };

    real get_value(context& data) const;

    void set_att(real time_seconds) { update_value(time_seconds, att_seconds); }
    void set_dec(real time_seconds) { update_value(time_seconds, dec_seconds); }
    void set_sus(real normalized) { sus_normalized = normalized; }
    void set_rel(real time_seconds) { update_value(time_seconds, rel_seconds); }

    //-------------------------------------------------------------------------
private:
    void update_value(real newValue, value& value);
    real attack(context& data) const;
    real decay(context& data) const;
    real sustain(context& data) const;
    real release(context& data) const;
    real shape(real xVal) const;

    value att_seconds;
    value dec_seconds;
    mut_real sus_normalized = real(0.);
    value rel_seconds;
};

//-----------------------------------------------------------------------------
class adsr_envelope_processor
{
public:
    //-------------------------------------------------------------------------
    adsr_envelope_processor() = default;

    void trigger();
    real read(real time_seconds) const;
    void release();

    void set_att(real value) { adsr.set_att(value); };
    void set_dec(real value) { adsr.set_dec(value); };
    void set_sus(real value) { adsr.set_sus(value); };
    void set_rel(real value) { adsr.set_rel(value); };

    //-------------------------------------------------------------------------
private:
    adsr_envelope adsr;
    mutable adsr_envelope::context current_data;
    mutable mut_real current_value = real(0.);
};

//-----------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha
