#include "audio_vgm_decoder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef USE_VGM_DECODER

extern "C" int vgm_play_start(const uint8_t *data, int size);
extern "C" int vgm_play_data(void *outBuffer, int size);
extern "C" int vgm_play_stop(void);
extern "C" int vgm_set_format(uint32_t frequency);

AudioVgmDecoder::~AudioVgmDecoder()
{
    if ( m_started )
    {
        vgm_play_stop();
        m_started = false;
    }
}

void AudioVgmDecoder::set_melody( const uint8_t *buffer, int size )
{
    if ( m_started )
    {
        vgm_play_stop();
        m_started = false;
    }
    vgm_set_format(m_rate);
    vgm_play_start(buffer, size);
    m_started = true;
}

void AudioVgmDecoder::set_format(uint32_t rate, uint8_t bps)
{
    m_rate = rate;
    m_bps = bps;
}

int AudioVgmDecoder::decode(uint8_t* origin_buffer, int max_size)
{
    int size = vgm_play_data(origin_buffer, max_size);
    if ( size == 0 )
    {
        vgm_play_stop();
        m_started = false;
    }
    return size;
}

#endif
