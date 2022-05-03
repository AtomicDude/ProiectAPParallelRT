#include "Camera.h"

namespace rt
{
    float rad(float deg)
    {
        return deg * TO_RAD;
    }

    Camera::Camera(const Vec3& eye, const Vec3& lookat, const Vec3& up, float fov, float ratio)
    {
        Vec3 v, u, w;

        float theta = rad(fov);
        float half_height = tanf(theta / 2.0f);
        float half_width = ratio * half_height;

        origin = eye;
        w = normalize(eye - lookat);
        u = normalize(cross(up, w));
        v = cross(w, u);

        Vec3 hwu = half_width * u;
        Vec3 hhv = half_height * v;

        lower_left = origin - (hwu + hhv + w);
        horizontal = 2.0f * hwu;
        vertical = 2.0f * hhv;
    }

    Ray Camera::castRay(float u, float v) const
    {
        return Ray(origin, lower_left + u * horizontal + v * vertical);
    }
}
