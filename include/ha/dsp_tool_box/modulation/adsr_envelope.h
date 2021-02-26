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
    using value = std::pair<float_t, float_t>;

    static constexpr float_t MIN_VALUE = 0.;
    static constexpr float_t MAX_VALUE = 1.;

    enum class stages
    {
        STAGE_BEFORE_TRIGGER,
        STAGE_ATTACK,
        STAGE_DECAY,
        STAGE_SUSTAIN,
        STAGE_RELEASE
    };

    struct context_data
    {
        stages stage;
        float_t time_seconds;
        float_t release_value;
    };

    float_t get_value(context_data& data) const;

    void set_att(float_t time_seconds) { update_value(time_seconds, att_seconds); }
    void set_dec(float_t time_seconds) { update_value(time_seconds, dec_seconds); }
    void set_sus(float_t normalized) { sus_normalized = normalized; }
    void set_rel(float_t time_seconds) { update_value(time_seconds, rel_seconds); }

    //-------------------------------------------------------------------------
private:
    void update_value(float_t newValue, value& value);
    float_t attack(context_data& data) const;
    float_t decay(context_data& data) const;
    float_t sustain(context_data& data) const;
    float_t release(context_data& data) const;
    float_t shape(float_t xVal) const;

    value att_seconds;
    value dec_seconds;
    float_t sus_normalized = float_t(0.);
    value rel_seconds;
};

//-----------------------------------------------------------------------------
class adsr_envelope_processor
{
public:
    //-------------------------------------------------------------------------
    adsr_envelope_processor() = default;

    void trigger();
    float_t read(float_t time_seconds) const;
    void release();

    void set_att(float_t value) { adsr.set_att(value); };
    void set_dec(float_t value) { adsr.set_dec(value); };
    void set_sus(float_t value) { adsr.set_sus(value); };
    void set_rel(float_t value) { adsr.set_rel(value); };

    //-------------------------------------------------------------------------
private:
    adsr_envelope adsr;
    mutable adsr_envelope::context_data current_data;
    mutable float_t current_value = float_t(0.);
};

//-----------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha
