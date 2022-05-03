#pragma once

#include "../Ray/Ray.h"

#define PI 3.14159265359f
#define TO_RAD (PI / 180.0f)

namespace rt
{
    float rad(float deg);

    class Camera
    {
    public:
        Camera(const Vec3& eye, const Vec3& lookat, const Vec3& up, float fov, float ratio);
        Ray castRay(float u, float v) const;

    private:
        Vec3 origin;
        Vec3 lower_left;
        Vec3 horizontal;
        Vec3 vertical;
    };
}