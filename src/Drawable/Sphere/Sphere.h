#pragma once

#include "../Drawable.h"

namespace rt
{
    class Sphere : public Drawable
    {
    public:
        Sphere(Vec3 center, double radius, const std::shared_ptr<Material>& material);
        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const override;

        const Vec3& center() const;
        double radius() const;

    private:
        Vec3 m_Center;
        double m_Radius;
    };
}