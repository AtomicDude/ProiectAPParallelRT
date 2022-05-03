#pragma once

#include "../Ray/Ray.h"

namespace rt
{
    struct HitRecord
    {
        float t;
        Vec3 p;
        Vec3 normal;
    };

    class Drawable
    {
    public:
        virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const = 0;
    };
}