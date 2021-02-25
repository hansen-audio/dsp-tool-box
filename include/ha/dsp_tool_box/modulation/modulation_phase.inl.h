// Copyright(c) 2021 Hansen Audio.

bool phase::check_overflow(value_type& phase_value)
{
    bool overflow = phase_value >= 1.f;
    if (overflow)
        phase_value = fmodf(phase_value, 1.f);

    return overflow;
}

//------------------------------------------------------------------------
bool phase::update(int num_samples)
{
    switch (mMode)
    {
        case kModeFree:
            update_free_running(num_samples);
            break;
        case kModeTempoSync:
            update_tempo_synced(num_samples);
            break;
        case kModeProjectSync:
            update_project_sync();
            break;
        default:
            assert(!"Invalid mode");
            break;
    }

    return check_overflow(current_phase);
}

//------------------------------------------------------------------------
phase::value_type phase::compute_free_running_factor(value_type rate,
                                                     value_type sample_rate_recip) const
{
    return rate * sample_rate_recip;
}

//------------------------------------------------------------------------
phase::value_type phase::compute_tempo_synced_factor(value_type sixty_seconds_recip,
                                                     value_type tempo) const
{
    return sixty_seconds_recip * tempo;
}

//------------------------------------------------------------------------
void phase::set_sample_rate(value_type value)
{
    sample_rate_recip   = 1.f / value;
    free_running_factor = compute_free_running_factor(rate, sample_rate_recip);
    tempo_synced_factor =
        free_running_factor * compute_tempo_synced_factor(creciprocal_60_seconds, tempo);
}

//------------------------------------------------------------------------
void phase::set_rate(value_type value)
{
    rate                = value;
    free_running_factor = compute_free_running_factor(rate, sample_rate_recip);
    tempo_synced_factor =
        free_running_factor * compute_tempo_synced_factor(creciprocal_60_seconds, tempo);
}

//------------------------------------------------------------------------
void phase::set_tempo(value_type value)
{
    tempo = value;
    tempo_synced_factor =
        free_running_factor * compute_tempo_synced_factor(creciprocal_60_seconds, tempo);
}

//------------------------------------------------------------------------
void phase::update_free_running(int num_samples)
{
    current_phase += static_cast<value_type>(num_samples) * free_running_factor;
}

//------------------------------------------------------------------------
void phase::update_tempo_synced(int num_samples)
{
    current_phase += static_cast<value_type>(num_samples) * tempo_synced_factor;
}

//------------------------------------------------------------------------
void phase::update_project_sync()
{
    current_phase = project_time * rate;
}
