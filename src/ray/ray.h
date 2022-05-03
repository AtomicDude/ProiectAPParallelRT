#pragma once

#include "../Vec/Vec3.h"

namespace rt
{
    class Ray
    {
    public:
        Ray(const Vec3& a, const Vec3& b);

        const Vec3& origin() const;
        const Vec3& direction() const;
        Vec3 point(float t) const;

    private:
        Vec3 o;
        Vec3 d;
    };
}