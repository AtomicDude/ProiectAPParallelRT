#include "Ray.h"

namespace rt
{
    Ray::Ray(const Vec3& origin, const Vec3& direction) :
        m_Origin(origin),
        m_Direction(direction)
    {
    }

    const Vec3& Ray::origin() const
    {
        return m_Origin;
    }

    const Vec3& Ray::direction() const
    {
        return m_Direction;
    }

    Vec3 Ray::point(double t) const
    {
        return m_Origin + t * m_Direction;
    }
}
