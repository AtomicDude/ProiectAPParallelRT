#pragma once

#include <random>
#include <math.h>

#define PI 3.14159265359
#define TO_RAD (PI / 180.0)

namespace rt
{
    inline double rad(double deg)
    {
        return deg * TO_RAD;
    }

    inline int32_t random_int(int32_t min = 0, int32_t max = RAND_MAX)
    {
        return min + rand() % (max - min + 1);
    }

    inline double random_double(double min = 0.0, double max = 1.0)
    {
        return min + static_cast<double>(rand()) / static_cast<double>(RAND_MAX + 1) * (max - min);
    }
}