#pragma once

#include "../Ray/Ray.h"
#include "../Material/Material.h"
#include "HitRecord.h"

namespace rt
{
    class Drawable
    {
    public:
        Drawable(const std::shared_ptr<Material>& material);
        virtual ~Drawable() = default;
        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const = 0;

    protected:
        std::shared_ptr<Material> m_Material;
    };
}