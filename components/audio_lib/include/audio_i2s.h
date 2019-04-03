#pragma once

#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"

class AudioI2S
{
public:
    AudioI2S() = default;
    ~AudioI2S() = default;

    void set_frequency(uint32_t frequency);

    void begin();
    /**
     * If nullptr is specified, I2S clears DMA buffer
     */
    int write(uint8_t* buffer, int len);
    void end();
};

