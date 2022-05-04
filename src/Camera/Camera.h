#pragma once

#include "../Ray/Ray.h"

namespace rt
{
    class Camera
    {
    public:
        Camera(const Vec3& eye, const Vec3& lookat, const Vec3& up, double fov, double ratio);
        Ray castRay(double u, double v) const;

    private:
        Vec3 m_Origin;
        Vec3 m_LowerLeft;
        Vec3 m_Horizontal;
        Vec3 m_Vertical;
    };
}