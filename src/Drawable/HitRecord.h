#pragma once

#include "../Vec/Vec3.h"

class Material;

namespace rt
{
    struct HitRecord
    {
        float t;
        Vec3 p;
        Vec3 normal;
        Material* material;
    };
}