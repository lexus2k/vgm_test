#include "audio_gme_decoder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern "C" int vgm_play_start(const uint8_t *data, int size);
extern "C" int vgm_play_data(void *outBuffer, int size);
extern "C" int vgm_play_stop(void);
extern "C" int vgm_set_format(uint32_t frequency);

#if 1

void AudioGmeDecoder::set_melody( const uint8_t *buffer, int size )
{
    vgm_set_format(m_rate);
    vgm_play_start(buffer, size);
}

void AudioGmeDecoder::set_format(uint32_t rate, uint8_t bps)
{
    m_rate = rate;
    m_bps = bps;
}

int AudioGmeDecoder::decode(uint8_t* origin_buffer, int max_size)
{
    int size = vgm_play_data(origin_buffer, max_size);
    if ( size == 0 )
    {
        vgm_play_stop();
    }
    return size;
}

#else

void AudioGmeDecoder::set_melody( const uint8_t *buffer, int size )
{
//    gme_open_file( filename, &m_emu, 44100 );
    if (m_emu != nullptr)
    {
        gme_delete( m_emu );
        m_emu = nullptr;
    }
    gme_open_data( buffer, size, &m_emu, m_rate );
    gme_start_track( m_emu, 0 );
}

void AudioGmeDecoder::set_format(uint32_t rate, uint8_t bps)
{
    m_rate = rate;
    m_bps = bps;
}

int AudioGmeDecoder::decode(uint8_t* origin_buffer, int max_size)
{
    gme_err_t err = gme_play( m_emu, max_size, (short int*)origin_buffer ) ;
    if (err != nullptr)
    {
        printf( err );
        return 0;
    }
    return max_size;
}

#endif
