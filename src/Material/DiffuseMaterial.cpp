#include "DiffuseMaterial.h"

namespace rt
{
    DiffuseMaterial::DiffuseMaterial(const Vec3& color):
        Material(color)
    {
    }

    bool DiffuseMaterial::scatter(const Ray& inRay, const Vec3& p, const Vec3& normal, bool isOutside, Vec3& outColor, Ray& outRay) const
    {
        Vec3 direction = normal + Vec3::randomUnit();

        if (Vec3::isNearZero(direction))
        {
            direction = normal;
        }

        outRay = Ray(p, direction);
        outColor = m_Color;

        return true;
    }
}