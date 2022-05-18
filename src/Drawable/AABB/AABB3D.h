#pragma once

#include "../Drawable.h"

namespace rt
{
    class AABB3D : public Drawable
    {
    public:
        AABB3D(const Vec3& min, double width, double height, double depth, const std::shared_ptr<Material>& material);
        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const override;

    private:
        Vec3 m_Min;
        double m_Width;
        double m_Height;
        double m_Depth;
    };
}