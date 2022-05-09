#pragma once

#include <stdint.h>

namespace rt
{
    struct Area
    {
        Area(uint32_t, uint32_t, uint32_t, uint32_t);
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
    };
}