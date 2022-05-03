#include "ray.h"

namespace rt
{
    ray::ray(const vec3& _a, const vec3& _b) :
        a(_a),
        b(_b)
    {
    }

    const vec3& ray::origin() const
    {
        return a;
    }

    const vec3& ray::direction() const
    {
        return b;
    }

    vec3 ray::point(float t) const
    {
        return a + t * b;
    }
}