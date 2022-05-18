#pragma once

#include "../Drawable.h"

namespace rt
{
    class AABB2D : public Drawable
    {
    public:
        AABB2D(const Vec3& min, double width, double height, const std::shared_ptr<Material>& material);
        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const = 0;

    protected:
        Vec3 m_Min;
        double m_Width;
        double m_Height;
    };

    class AABB2Dxy : public AABB2D
    {
    public:
        AABB2Dxy(const Vec3& min, double width, double height, const std::shared_ptr<Material>& material);
        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const override;
    };

    class AABB2Dxz : public AABB2D
    {
    public:
        AABB2Dxz(const Vec3& min, double width, double height, const std::shared_ptr<Material>& material);
        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const override;
    };

    class AABB2Dzy : public AABB2D
    {
    public:
        AABB2Dzy(const Vec3& min, double width, double height, const std::shared_ptr<Material>& material);
        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const override;
    };
}