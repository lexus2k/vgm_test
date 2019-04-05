#include "audio_i2s.h"
#include "audio_notes_decoder.h"
#include "nixie_melodies.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_adc_cal.h"

//#define I2S_DEBUG

AudioI2S::AudioI2S(uint32_t frequency)
    : m_frequency( frequency )
{
}

void AudioI2S::set_frequency(uint32_t frequency)
{
    m_frequency = frequency;
}

int AudioI2S::set_prebuffering(int prebuffering_ms)
{
    // 1920 for 16kHz
    // 8 * 2056 = 2048
    uint32_t bytes = m_frequency * prebuffering_ms / 1000 * 2 * 2;
    int i=5;
    while ( bytes > (1<<i) ) i++;
    m_buffer_size = 1<<i;
    return prebuffering_ms;
}

void AudioI2S::begin()
{
    i2s_config_t i2s_config{};
    i2s_config.mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN);
    i2s_config.sample_rate = m_frequency;
    i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    i2s_config.channel_format = I2S_CHANNEL_FMT_ALL_RIGHT;
    i2s_config.communication_format = static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_I2S_MSB);
    i2s_config.intr_alloc_flags = 0; //ESP_INTR_FLAG_LEVEL1;
    i2s_config.dma_buf_count = 8;
    i2s_config.dma_buf_len = m_buffer_size / 8;
    i2s_config.use_apll = false;
    esp_err_t err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    if (err != ESP_OK)
    {
        printf("error: %i\n", err);
    }
//    i2s_set_pin(I2S_NUM_0, NULL);
    i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);
    i2s_set_sample_rates(I2S_NUM_0, m_frequency);
    i2s_zero_dma_buffer( I2S_NUM_0 );
}

int AudioI2S::write(uint8_t* buffer, int len)
{
    size_t written = 0;
    if ( buffer == nullptr )
    {
        i2s_zero_dma_buffer( I2S_NUM_0 );
    }
    else
    {
        esp_err_t err = i2s_write(I2S_NUM_0, buffer, len, &written, 0); //portMAX_DELAY);
        if (err != ESP_OK)
        {
            return -1;
        }
#ifdef I2S_DEBUG
        printf("i2c: %u\n", written);
#endif
    }
    return written;
}

void AudioI2S::end()
{
    i2s_driver_uninstall(I2S_NUM_0);
}


/*
...

    i2s_driver_install(i2s_num, &i2s_config, 0, NULL);   //install and start i2s driver

    i2s_set_pin(i2s_num, NULL); //for internal DAC, this will enable both of the internal channels

    //You can call i2s_set_dac_mode to set built-in DAC output mode.
    //i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);

    i2s_set_sample_rates(i2s_num, 22050); //set sample rates

    i2s_driver_uninstall(i2s_num); //stop & destroy i2s driver

*/
