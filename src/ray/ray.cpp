#include "Ray.h"

namespace rt
{
    Ray::Ray(const Vec3& a, const Vec3& b) :
        o(a),
        d(b - a)
    {
    }

    const Vec3& Ray::origin() const
    {
        return o;
    }

    const Vec3& Ray::direction() const
    {
        return d;
    }

    Vec3 Ray::point(float t) const
    {
        return o + t * d;
    }
}