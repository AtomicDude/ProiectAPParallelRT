#pragma once

#include "../Vec/Vec3.h"
#include "../Material/Material.h"

#include <memory>

namespace rt
{
    struct HitRecord
    {
        double t;
        Vec3 p;
        Vec3 normal;
        std::shared_ptr<Material> material;
    };
}