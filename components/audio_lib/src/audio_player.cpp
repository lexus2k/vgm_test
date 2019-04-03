#include "audio_player.h"
#include "audio_gme_decoder.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 4096

//#define AUDIO_PLAYER_DEBUG

void AudioPlayer::begin()
{
    m_output.set_frequency( m_frequency );
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
        m_write_pos = m_buffer;
        m_player_pos = m_buffer;
        m_end_detected = false;
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
        m_write_pos = m_buffer;
        m_player_pos = m_buffer;
        m_end_detected = false;
}

bool AudioPlayer::update()
{
    if (m_decoder == nullptr)
    {
        return false;
    }
    if (m_buffer == nullptr)
    {
        m_buffer = static_cast<uint8_t*>(malloc(BUFFER_SIZE));
        m_write_pos = m_buffer;
        m_player_pos = m_buffer;
        m_end_detected = false;
    }
    if (!m_end_detected)
    {
        size_t size;
        if (m_player_pos > m_write_pos)
        {
            size = m_player_pos - m_write_pos;
        }
        else
        {
            size = m_buffer + BUFFER_SIZE - m_write_pos;
        }
        size = (size >> 2) << 2;
        if ( size )
        {
            size = m_decoder->decode( m_write_pos, size );
            if ( size == 0 )
            {
                m_end_detected = true;
            }
            m_write_pos += size;
            if ( m_write_pos == m_buffer + BUFFER_SIZE )
            {
                m_write_pos = m_buffer;
            }
        }
    }
    if ( m_player_pos == m_write_pos && m_end_detected )
    {
        // to clear i2s dma buffer
        m_output.write( nullptr, 0 );
        return false;
    }
    int size;
    if ( m_player_pos >= m_write_pos )
    {
        size = m_buffer + BUFFER_SIZE - m_player_pos;
    }
    else
    {
        size = m_write_pos - m_player_pos;
    }
    int written = m_output.write(m_player_pos, size);
    if (written < 0)
    {
        // some critical error happened
        return false;
    }
    #ifdef AUDIO_PLAYER_DEBUG
    else
    {
        for(int i=0; i<written; i++)
        {
            fprintf( stderr, "%02X ", m_player_pos[i] );
        }
        fprintf( stderr, "\n" );
    }
    #endif
    m_player_pos += written;
    if ( m_player_pos == m_buffer + BUFFER_SIZE )
    {
        m_player_pos = m_buffer;
    }
    if ( written < size )
    {
//        break;
    }
    return true;
}

