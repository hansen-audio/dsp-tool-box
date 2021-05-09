// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/dsp_tool_box/core/types.h"

namespace ha {
namespace dtb {
namespace modulation {

/**
 * @brief Phase running from 0 to 1 and can be used for e.g. an LFO
 */
struct phase
{
    /**
     * @brief Sync mode of the phase
     */
    enum class sync_mode
    {
        FREE = 0,
        TEMPO_SYNC,
        PROJECT_SYNC
    };

    /**
     * @brief Context of a phase
     */
    struct context
    {
        mut_real rate                = real(0.);
        mut_real tempo               = real(120.);
        mut_real sample_rate_recip   = real(1.);
        mut_real project_time        = real(0.);
        sync_mode mode               = sync_mode::FREE;
        mut_real free_running_factor = real(0.);
        mut_real tempo_synced_factor = real(0.);
    };

    /**
     * @brief Advances the phase value according to the given context
     *
     * @param value Current phase value
     * @param num_samples Number of samples to advance
     * @return Returns true once overflown
     */
    static bool
    advance_one_shot(context const& cx, mut_real& value, i32 num_samples);

    /**
     * @brief Advances the phase value according to the given context
     *
     * @param value Current phase value
     * @param num_samples Number of samples to advance
     * @return Returns true on overflow
     */
    static bool advance(context const& cx, mut_real& value, i32 num_samples);

    /**
     * @brief Sets the sync mode of context
     *
     * @param value \sa sync_mode
     */
    static void set_sync_mode(context& cx, sync_mode value);

    /**
     * @brief Sets the tempo of context
     *
     * @param value Tempo in [bpm], e.g. 120
     */
    static void set_tempo(context& cx, real value);

    /**
     * @brief Sets the rate of context
     *
     * @param value \sa note_length_to_rate
     */
    static void set_rate(context& cx, real value);

    /**
     * @brief Sets the sample rate of context
     *
     * @param value Sample rate in [Hz]
     */
    static void set_sample_rate(context& cx, real value);

    /**
     * @brief Sets the project time of context
     *
     * @param value Project time in musical beats
     */
    static void set_project_time(context& cx, real value);

    /**
     * @brief Sets the note len of context
     *
     * @param value Note len e.g. 1/32 -> 0.03125
     */
    static void set_note_length(context& cx, real value);

    /**
     * @brief Converts note len to rate (call set_rate(...) afterwards)
     *
     * @param value value Note len e.g. 1/32 -> 0.03125
     */
    static real note_length_to_rate(real value);
};

//------------------------------------------------------------------------
} // namespace modulation
} // namespace dtb
} // namespace ha
