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

AudioPlayer audio_player;
extern const uint8_t test_vgm_start[] asm("_binary_test_vgm_start");
extern const uint8_t test_vgm_end[]   asm("_binary_test_vgm_end");

#if 0

void buttons_start_audio(void)
{
//    audio_player.play( &melodyMonkeyIslandP );
}
#endif

static void main_task(void *pvParameter)
{
    audio_player.begin();
    audio_player.playVGM( test_vgm_start, test_vgm_end - test_vgm_start );
    for(;;)
    {
        audio_player.update();
        vTaskDelay(2);
//        esp_task_wdt_reset();
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
