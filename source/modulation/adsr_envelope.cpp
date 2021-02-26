// Copyright René Hansen 2016.

#include "ha/dsp_tool_box/modulation/adsr_envelope.h"

namespace ha {
namespace dtb {
namespace modulation {

//-----------------------------------------------------------------------------
//	adsr_envelope
//-----------------------------------------------------------------------------
float_t adsr_envelope::get_value(context_data& data) const
{
    float_t value = MIN_VALUE;
    switch (data.stage)
    {
        case stages::STAGE_SUSTAIN:
            value = sustain(data);
            break;
        case stages::STAGE_ATTACK:
            value = attack(data);
            break;
        case stages::STAGE_DECAY:
            value = decay(data);
            break;
        case stages::STAGE_RELEASE:
            value = release(data);
            break;
        case stages::STAGE_BEFORE_TRIGGER:
        default:
            value = MIN_VALUE;
            break;
    }

    return value;
}

//-----------------------------------------------------------------------------
void adsr_envelope::update_value(float_t new_value, value& value)
{
    if (value.first == new_value)
        return;

    value.first  = new_value;
    value.second = (new_value > float_t(0.)) ? (float_t(1.) / new_value) : float_t(0.);
}

//-----------------------------------------------------------------------------
float_t adsr_envelope::attack(context_data& data) const
{
    data.stage = stages::STAGE_ATTACK;
    if (data.time_seconds > att_seconds.first)
        return decay(data);

    return data.time_seconds * att_seconds.second;
}

//-----------------------------------------------------------------------------
float_t adsr_envelope::decay(context_data& data) const
{
    data.stage = stages::STAGE_DECAY;
    data.time_seconds -= att_seconds.first;
    if (data.time_seconds > dec_seconds.first)
        return sustain(data);

    float_t value = data.time_seconds * dec_seconds.second;
    return (sus_normalized - MAX_VALUE) * shape(value) + MAX_VALUE;
}

//-----------------------------------------------------------------------------
inline float_t adsr_envelope::sustain(context_data& data) const
{
    data.stage = stages::STAGE_SUSTAIN;
    return sus_normalized;
}

//-----------------------------------------------------------------------------
float_t adsr_envelope::release(context_data& data) const
{
    /*!
    The Fourth cycle is 'release' starting when key is released.
    It runs to 0 beginning from the value the envelope had when
    releasing the key. Often this is 'sustain' value.
    */
    data.stage = stages::STAGE_RELEASE;
    if (data.time_seconds > rel_seconds.first)
        return MIN_VALUE;

    float_t value = data.time_seconds * rel_seconds.second;
    return data.release_value - shape(value) * data.release_value;
}

//-----------------------------------------------------------------------------
float_t adsr_envelope::shape(float_t x_val) const
{
    return easing::ease_virus_ti<float_t>(x_val);
}

//-----------------------------------------------------------------------------
//	adsr_envelope_processor
//-----------------------------------------------------------------------------
void adsr_envelope_processor::trigger()
{
    current_data.stage         = adsr_envelope::stages::STAGE_ATTACK;
    current_data.release_value = adsr_envelope::MAX_VALUE;
    current_data.time_seconds  = float_t(0.);
}

//-----------------------------------------------------------------------------
float_t adsr_envelope_processor::read(float_t time_seconds) const
{
    current_data.time_seconds = time_seconds;
    current_value             = adsr.get_value(current_data);
    return current_value;
}

//-----------------------------------------------------------------------------
void adsr_envelope_processor::release()
{
    current_data.stage         = adsr_envelope::stages::STAGE_RELEASE;
    current_data.release_value = current_value;
    current_data.time_seconds  = float_t(0.);
}

//-----------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha
