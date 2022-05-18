#pragma once

#include "../Drawable.h"

namespace rt
{
    class AABB2D : public Drawable
    {
    public:
        AABB2D(const Vec3& min, double width, double height, const std::shared_ptr<Material>& material);
        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const override;

    private:
        Vec3 m_Min;
        double m_Width;
        double m_Height;
    };
}