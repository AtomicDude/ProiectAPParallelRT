#pragma once

#include "../Ray/Ray.h"

namespace rt
{
    class Material
    {
    public:
        Material(const Vec3& color);

        virtual bool scatter(const Ray& inRay, const Vec3& p, const Vec3& normal, Vec3& outColor, Ray& outRay) const = 0;

    protected:
        Vec3 m_Color;
    };
}