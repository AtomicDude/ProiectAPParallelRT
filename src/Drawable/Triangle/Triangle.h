#pragma once

#include "../Drawable.h"

namespace rt
{
    class Triangle : public Drawable
    {
    public:
        Triangle(const Vec3& a, const Vec3& b, const Vec3& c, const std::shared_ptr<Material>& material);
        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const override;

    private:
        Vec3 m_A;
        Vec3 m_B;
        Vec3 m_C;
        Vec3 m_N;
    };
}