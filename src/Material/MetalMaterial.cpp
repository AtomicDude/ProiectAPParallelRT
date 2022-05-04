#include "MetalMaterial.h"

namespace rt
{
    MetalMaterial::MetalMaterial(const Vec3& color, double fuzziness):
        Material(color),
        m_Fuzziness(fuzziness)
    {
    }

    bool MetalMaterial::scatter(const Ray& inRay, const Vec3& p, const Vec3& normal, Vec3& outColor, Ray& outRay) const
    {
        Vec3 direction = Vec3::reflect(Vec3::normalize(inRay.direction()), normal);
        outRay = Ray(p, direction + m_Fuzziness * Vec3::randomInUnitSphere());
        outColor = m_Color;

        return Vec3::dot(outRay.direction(), normal) > 0.0;
    }
}