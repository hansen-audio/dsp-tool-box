// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "ha/dsp_tool_box/core/types.h"

namespace ha::dtb::modulation {

/**
 * @brief Phase running from 0 to 1 and can be used for e.g. an LFO
 */
struct Phase final
{
    /**
     * @brief Sync mode of the phase
     */
    enum class SyncMode
    {
        Free = 0,
        TempoSync,
        ProjectSync
    };

    mut_real rate                = real(0.);
    mut_real tempo               = real(120.);
    mut_real sample_rate_recip   = real(1.);
    mut_real project_time        = real(0.);
    SyncMode mode                = SyncMode::Free;
    mut_real free_running_factor = real(0.);
    mut_real tempo_synced_factor = real(0.);
    mut_real note_len            = real(1. / 32.);
};

struct PhaseImpl final
{
    /**
     * @brief Create a Phase
     *
     * @return Returns a fully initialised and functional Phase
     */
    static Phase create();

    /**
     * @brief Advances the phase value in a one shot like way. Once overflown
     * the phase keeps being 1.
     *
     * @param value Current phase value
     * @param num_samples Number of samples to advance
     * @return Returns true once overflown
     */
    static bool
    advance_one_shot(Phase const& self, mut_real& value, i32 num_samples);

    /**
     * @brief Advances the phase value. When overflown it starts at 0 again.
     *
     * @param value Current phase value
     * @param num_samples Number of samples to advance
     * @return Returns true on overflow
     */
    static bool advance(Phase const& self, mut_real& value, i32 num_samples);

    /**
     * @brief Sets the sync mode of Phase
     *
     * @param value \sa sync_mode
     */
    static void set_sync_mode(Phase& self, Phase::SyncMode value);

    /**
     * @brief Sets the tempo of Phase
     *
     * @param value Tempo in [bpm], e.g. 120
     */
    static void set_tempo(Phase& self, real value);

    /**
     * @brief Sets the rate of Phase
     *
     * @param value \sa note_length_to_rate
     */
    static void set_rate(Phase& self, real value);

    /**
     * @brief Sets the sample rate of Phase
     *
     * @param value Sample rate in [Hz]
     */
    static void set_sample_rate(Phase& self, real value);

    /**
     * @brief Sets the project time of Phase
     *
     * @param value Project time in musical beats
     */
    static void set_project_time(Phase& self, real value);

    /**
     * @brief Sets the note len of Phase
     *
     * @param value Note len e.g. 1/32 -> 0.03125
     */
    static void set_note_len(Phase& self, real value);

    /**
     * @brief Converts note len to rate (call set_rate(...) afterwards)
     *
     * @param value value Note len e.g. 1/32 -> 0.03125
     */
    static real note_length_to_rate(real value);
};

//------------------------------------------------------------------------
} // namespace ha::dtb::modulation
