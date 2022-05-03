#pragma once

#include "../Ray/Ray.h"
#include "HitRecord.h"

namespace rt
{
    class Drawable
    {
    public:
        virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const = 0;
    };
}