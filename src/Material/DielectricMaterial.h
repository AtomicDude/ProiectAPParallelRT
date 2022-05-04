#pragma once

#include "Material.h"
#include "../Utility/Utility.h"

namespace rt
{
    class DielectricMaterial : public Material
    {
    public:
        DielectricMaterial(const Vec3& color, double refraction_index = 1.0);

        virtual bool scatter(const Ray& inRay, const Vec3& p, const Vec3& normal, bool isOutside, Vec3& outColor, Ray& outRay) const override;

    private:
        double m_RefractionIndex;
    };
}