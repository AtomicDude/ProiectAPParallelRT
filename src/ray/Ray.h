#pragma once

#include "../Vec/Vec3.h"

namespace rt
{
    class Ray
    {
    public:
        Ray(const Vec3& origin = 0.0, const Vec3& direction = 0.0);

        const Vec3& origin() const;
        const Vec3& direction() const;
        Vec3 point(double t) const;

    private:
        Vec3 m_Origin;
        Vec3 m_Direction;
    };
}
