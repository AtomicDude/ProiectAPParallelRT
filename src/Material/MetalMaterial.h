#pragma once

#include "Material.h"

namespace rt
{
    class MetalMaterial : public Material
    {
    public:
        MetalMaterial(const Vec3& color, double fuzziness = 0.0);

        virtual bool scatter(const Ray& inRay, const Vec3& p, const Vec3& normal, Vec3& outColor, Ray& outRay) const override;

    private:
        double m_Fuzziness;
    };
}