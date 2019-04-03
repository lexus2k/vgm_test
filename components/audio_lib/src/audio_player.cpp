#include "audio_player.h"
#include "audio_gme_decoder.h"
#include <stdio.h>
#include <string.h>

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

void AudioPlayer::play(const NixieMelody* melody)
{
    if (m_decoder != nullptr)
    {
        delete m_decoder;
    }
    AudioNotesDecoder* decoder = new AudioNotesDecoder();
    // TODO get format from m_output
    decoder->set_format(16000, 16);
    decoder->set_melody( melody );
    m_decoder = decoder;
}

void AudioPlayer::playVGM(const uint8_t *buffer, int size)
{
    if (m_decoder != nullptr)
    {
        delete m_decoder;
    }
    AudioGmeDecoder* decoder = new AudioGmeDecoder();
    // TODO get format from m_output
    decoder->set_format(8000, 16);
    decoder->set_melody( buffer, size );
    m_decoder = decoder;
}

bool AudioPlayer::update()
{
    if (m_decoder == nullptr)
    {
        return false;
    }
    if (m_buffer == nullptr)
    {
        m_buffer = static_cast<uint8_t*>(malloc(2048));
        m_size = 0;
    }
    if (m_size == 0)
    {
        m_size = m_decoder->decode(m_buffer, 2048);
        if (m_size == 0)
        {
            // to clear i2s dma buffer
            m_output.write( nullptr, 0 );
            return false;
        }
        m_player_pos = m_buffer;
    }
    int written = m_output.write(m_player_pos, m_size);
    if (written < 0)
    {
        // some critical error happened
        return false;
    }
    else
    {
        for(int i=0; i<written; i++)
        {
            fprintf( stderr, "%02X ", m_player_pos[i] );
        }
        fprintf( stderr, "\n" );
    }
    m_player_pos += written;
    m_size -= written;
    return true;
}

