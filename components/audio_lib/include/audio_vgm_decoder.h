#pragma once

#include "audio_decoder.h"
#include <stdint.h>

#ifdef USE_VGM_DECODER

class AudioVgmDecoder: public AudioDecoder
{
public:
    AudioVgmDecoder() = default;
    virtual ~AudioVgmDecoder();

    void set_melody( const uint8_t *buffer, int size );
    void set_format(uint32_t rate, uint8_t bps);
    int decode(uint8_t* buffer, int max_size) override;
private:
    uint32_t m_rate = 16000;
    uint8_t m_bps = 16;
    bool m_started = false;
};

#endif
