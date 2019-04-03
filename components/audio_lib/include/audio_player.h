#pragma once

#include "audio_i2s.h"
#include "audio_notes_decoder.h"
#include "audio_gme_decoder.h"

class AudioPlayer
{
public:
    AudioPlayer() = default;
    ~AudioPlayer() = default;

    void play(const NixieMelody* melody);
    void playVGM(const uint8_t *buffer, int size);
    void begin();
    void end();
    /**
     * Return false, when nothing is played
     */
    bool update();

private:
    AudioI2S m_output;
    AudioDecoder* m_decoder = nullptr;
    uint32_t m_frequency = 16000;
    uint8_t* m_buffer = nullptr;
    uint8_t* m_write_pos = nullptr;
    uint8_t* m_player_pos = nullptr;
    bool m_end_detected = false;
};
