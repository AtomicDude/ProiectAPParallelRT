#pragma once

#include "vec3.h"

namespace rt
{
    class ray
    {
    public:
        ray(const vec3& _a, const vec3& _b);

        const vec3& origin() const;
        const vec3& direction() const;
        vec3 point(float t) const;

    private:
        vec3 a;
        vec3 b;
    };
}