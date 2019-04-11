/*
    This file is part of I2S demo player for ESP32.
    Copyright (C) 2019  Alexey Dynda.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "audio_player.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "soc/io_mux_reg.h"
#include "driver/ledc.h"
#include "driver/adc.h"
#include "esp_task_wdt.h"

AudioPlayer audio_player( 16000 );
extern const uint8_t test1_vgm_start[] asm("_binary_test1_vgm_start");
extern const uint8_t test1_vgm_end[]   asm("_binary_test1_vgm_end");
extern const uint8_t test2_vgm_start[] asm("_binary_test2_vgm_start");
extern const uint8_t test2_vgm_end[]   asm("_binary_test2_vgm_end");
extern const uint8_t test3_vgm_start[] asm("_binary_test3_vgm_start");
extern const uint8_t test3_vgm_end[]   asm("_binary_test3_vgm_end");
extern const uint8_t test4_vgm_start[] asm("_binary_test4_vgm_start");
extern const uint8_t test4_vgm_end[]   asm("_binary_test4_vgm_end");
extern const uint8_t test5_vgm_start[] asm("_binary_test5_vgm_start");
extern const uint8_t test5_vgm_end[]   asm("_binary_test5_vgm_end");

static const uint8_t *melodies[][2] =
{
    { test1_vgm_start, test1_vgm_end },
    { test2_vgm_start, test2_vgm_end },
    { test3_vgm_start, test3_vgm_end },
    { test4_vgm_start, test4_vgm_end },
    { test5_vgm_start, test5_vgm_end },
};

static void main_task(void *pvParameter)
{
    int index = 0;
    /* set prebuffering in milliseconds. It is require, when thread sleeps */
    audio_player.set_prebuffering( 50 );
    audio_player.begin();
//    audio_player.play_vgm( melodies[index][0], melodies[index][1] - melodies[index][0] );
    for(;;)
    {
        if ( !audio_player.update() )
        {
            audio_player.play_vgm( melodies[index][0], melodies[index][1] - melodies[index][0] );
            index++; if ( index > 4 ) index = 0;
            audio_player.update();
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }

    audio_player.end();
    fflush(stdout);
    esp_restart();
}

extern "C" void app_main()
{
    xTaskCreate(&main_task, "main_task", 4096 /* 4096 */, NULL, 5, NULL);
    for(;;)
    {
        esp_task_wdt_reset();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
