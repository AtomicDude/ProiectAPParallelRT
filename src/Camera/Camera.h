#pragma once

#include "../Ray/Ray.h"

namespace rt
{
    class Camera
    {
    public:
        Camera(
            const Vec3& eye,
            const Vec3& lookat,
            const Vec3& up,
            double fov,
            double ratio,
            double aperture = 0.0
        );
        Ray castRay(double s, double t) const;

    private:
        Vec3 m_Origin;
        Vec3 m_LowerLeft;
        Vec3 m_Horizontal;
        Vec3 m_Vertical;
        Vec3 m_U;
        Vec3 m_V;
        Vec3 m_W;
        double m_LensRadius;
    };
}