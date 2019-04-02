#pragma once

#include <stdint.h>

class AudioDecoder
{
public:
    AudioDecoder() = default;
    virtual ~AudioDecoder() = default;
    virtual int decode(uint8_t* buffer, int max_size) = 0;
};

