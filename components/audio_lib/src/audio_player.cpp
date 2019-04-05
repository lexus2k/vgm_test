#include "audio_player.h"
#include "audio_gme_decoder.h"
#include <stdio.h>
#include <string.h>

//#define AUDIO_PLAYER_DEBUG

AudioPlayer::AudioPlayer(uint32_t frequency)
   : m_output( frequency )
   , m_frequency( frequency )
{
    set_prebuffering( 20 );
}

void AudioPlayer::begin()
{
    m_output.begin();
}

void AudioPlayer::end()
{
    if (m_buffer != nullptr )
    {
        free(m_buffer);
        m_buffer = nullptr;
    }
    m_output.end();
}

void AudioPlayer::set_prebuffering(int prebuffering_ms)
{
    m_output.set_prebuffering( prebuffering_ms );
    uint32_t bytes = m_frequency * prebuffering_ms / 1000 * 2 * 2;
    int i=5;
    while ( bytes > (1<<i) ) i++;
    // We do not need large buffer for decoder
    m_max_buffer_size = 1<<(i - 1);
}

void AudioPlayer::play(const NixieMelody* melody)
{
    if (m_decoder != nullptr)
    {
        delete m_decoder;
    }
    AudioNotesDecoder* decoder = new AudioNotesDecoder();
    // TODO get format from m_output
    decoder->set_format(m_frequency, 16);
    decoder->set_melody( melody );
    m_decoder = decoder;
    reset_player();
}

void AudioPlayer::playVGM(const uint8_t *buffer, int size)
{
    if (m_decoder != nullptr)
    {
        delete m_decoder;
    }
    AudioGmeDecoder* decoder = new AudioGmeDecoder();
    // TODO get format from m_output
    decoder->set_format( m_frequency, 16 );
    decoder->set_melody( buffer, size );
    m_decoder = decoder;
    reset_player();
}

int AudioPlayer::reset_player()
{
    if (m_buffer != nullptr)
    {
        free( m_buffer );
        m_buffer = nullptr;
    }
    m_buffer = static_cast<uint8_t*>(malloc(m_max_buffer_size));
    m_write_pos = m_buffer;
    m_player_pos = m_buffer;
    m_size = 0;
    return 0;
}

int AudioPlayer::decode_data()
{
    uint8_t *end = m_buffer + m_max_buffer_size;
    int size = m_max_buffer_size - m_size;
    if ( size > end - m_write_pos )
    {
        size = end - m_write_pos;
    }
    if ( size )
    {
        size = m_decoder->decode( m_write_pos, size );
        if ( size >= 0 )
        {
            m_write_pos += size;
            m_size += size;
            if ( m_write_pos == end )
            {
                m_write_pos = m_buffer;
            }
        }
    }
    return size;
}

int AudioPlayer::play_data()
{
    uint8_t *end = m_buffer + m_max_buffer_size;
    int size = m_size;
    if ( size > end - m_player_pos )
    {
        size = end - m_player_pos;
    }
    if ( size == 0 )
    {
        return 0;
    }
    int written = m_output.write(m_player_pos, size);
    if (written >= 0)
    {
        m_player_pos += written;
        m_size -= written;
        if ( m_player_pos == end )
        {
            m_player_pos = m_buffer;
        }
    #ifdef AUDIO_PLAYER_DEBUG
        for(int i=0; i<written; i++)
        {
            fprintf( stderr, "%02X ", m_player_pos[i] );
        }
        fprintf( stderr, "\n" );
    #endif
    }
    return written;
}

bool AudioPlayer::update()
{
    if (m_decoder == nullptr)
    {
        return false;
    }
    int played = 0, decoded = 0;
    do
    {
        played = play_data();
        if ( played < 0 )
        {
            delete m_decoder;
            m_decoder = nullptr;
            return false;
        }
        decoded = decode_data();
        if ( decoded < 0 )
        {
            delete m_decoder;
            m_decoder = nullptr;
            return false;
        }
    } while ( played > 0 || decoded > 0 );
    if ( m_size == 0 )
    {
        // to clear i2s dma buffer
        m_output.write( nullptr, 0 );
        delete m_decoder;
        m_decoder = nullptr;
        return false;
    }
    return true;
}

