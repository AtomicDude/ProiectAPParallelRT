#include "DielectricMaterial.h"

#include "../Utility/Utility.h"

namespace rt
{
    DielectricMaterial::DielectricMaterial(const Vec3& color, double refraction_index) :
        Material(color),
        m_RefractionIndex(refraction_index)
    {
    }

    bool DielectricMaterial::scatter(const Ray& inRay, const Vec3& p, const Vec3& normal, bool isOutside, Vec3& outColor, Ray& outRay) const
    {
        double ratio = isOutside ? (1.0 / m_RefractionIndex) : m_RefractionIndex;

        Vec3 unit_direction = Vec3::normalize(inRay.direction());

        double cos_theta = fmin(Vec3::dot(-unit_direction, normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool no_refract = ratio * sin_theta > 1.0;
        
        double reflectance = (1.0 - ratio) / (1.0 + ratio);
        reflectance *= reflectance;
        reflectance += (1.0 - reflectance) * pow(1.0 - cos_theta, 5.0);

        bool reflect = reflectance > random_double(0.0, 1.0);

        Vec3 direction;

        if (no_refract || reflect)
        {
            direction = Vec3::reflect(unit_direction, normal);
        }
        else
        {
            direction = Vec3::refract(unit_direction, normal, ratio);
        }

        outRay = Ray(p, direction);
        outColor = m_Color;

        return true;
    }
}