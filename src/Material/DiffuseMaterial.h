#pragma once

#include "Material.h"

namespace rt
{
    class DiffuseMaterial : public Material
    {
    public:
        DiffuseMaterial(const Vec3& color);

        virtual bool scatter(const Ray& inRay, const Vec3& p, const Vec3& normal, bool isOutside, Vec3& outColor, Ray& outRay) const override;
    };
}