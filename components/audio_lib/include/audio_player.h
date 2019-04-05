#pragma once

#include "audio_notes_decoder.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include <stdint.h>
#include <string.h>

class AudioPlayer
{
public:
    AudioPlayer(uint32_t frequency = 16000);
    ~AudioPlayer() = default;

    void play(const NixieMelody* melody);
    void play_vgm(const uint8_t *buffer, int size);
    void begin();
    void end();
    void set_on_play_complete( void (*cb)() = nullptr ) { m_on_play_complete = cb; }
    /**
     * Return false, when nothing is played
     */
    bool update();
    void set_prebuffering(int prebuffering_ms);

private:
    AudioDecoder* m_decoder = nullptr;
    void (*m_on_play_complete)() = nullptr;
    uint32_t m_frequency = 16000;
    uint8_t* m_buffer = nullptr;
    uint32_t m_i2s_buffer_size = 512;
    uint32_t m_decoder_buffer_size = 2048;
    uint8_t* m_write_pos = nullptr;
    uint8_t* m_player_pos = nullptr;
    int m_size;

    int reset_player();
    int decode_data();
    int play_data();
};
