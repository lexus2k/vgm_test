#pragma once

#include "nixie_audio_defs.h"
#include "audio_decoder.h"
#include <stdint.h>

class AudioNotesDecoder: public AudioDecoder
{
public:
    AudioNotesDecoder() = default;
    virtual ~AudioNotesDecoder() = default;

    void set_format(uint32_t rate, uint8_t bps);
    void set_melody( const NixieMelody* melody );

    int decode(uint8_t* buffer, int max_size) override;

private:
    const NixieMelody* m_melody = nullptr;
    uint32_t m_rate = 16000;
    uint8_t m_bps = 16;
    const uint8_t *m_position = nullptr;
    uint16_t m_note_samples_left = 0;
    uint16_t m_played_period = 0;
    uint16_t m_samples_per_period = 0;
    uint16_t m_pause_left = 0;
    uint16_t m_last_index = 0;

    bool read_note_data();
    void next_note();
};
