#include "AABB2D.h"
#include "../Triangle/Triangle.h"
#include <math.h>

namespace rt
{
    AABB2D::AABB2D(const Vec3& min, double width, double height, const std::shared_ptr<Material>& material):
        Drawable(material),
        m_Min(min),
        m_Width(width),
        m_Height(height)
    {
    }

    AABB2Dxy::AABB2Dxy(const Vec3& min, double width, double height, const std::shared_ptr<Material>& material):
        AABB2D(min, width, height, material)
    {
    }

    bool AABB2Dxy::hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
    {
        const Vec3& min = m_Min;
        Vec3 max = Vec3(min.x + m_Width, min.y + m_Height, min.z);

        double t = (min.z - ray.origin().z) / ray.direction().z;

        if (t >= t_min && t <= t_max)
        {
            double x = ray.origin().x + t * ray.direction().x;
            double y = ray.origin().y + t * ray.direction().y;

            if (x >= min.x && x <= max.x && y >= min.y && y <= max.y)
            {
                hit_record.t = t;
                hit_record.p = ray.point(t);
                hit_record.normal = Vec3(0, 0, 1);
                hit_record.material = m_Material;

                return true;
            }
        }

        return false;
    }

    AABB2Dxz::AABB2Dxz(const Vec3& min, double width, double height, const std::shared_ptr<Material>& material):
        AABB2D(min, width, height, material)
    {
    }

    bool AABB2Dxz::hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
    {
        const Vec3& min = m_Min;
        Vec3 max = Vec3(min.x + m_Width, min.y, min.z + m_Height);

        double t = (min.y - ray.origin().y) / ray.direction().y;

        if (t >= t_min && t <= t_max)
        {
            double x = ray.origin().x + t * ray.direction().x;
            double z = ray.origin().z + t * ray.direction().z;

            if (x >= min.x && x <= max.x && z >= min.z && z <= max.z)
            {
                hit_record.t = t;
                hit_record.p = ray.point(t);
                hit_record.normal = Vec3(0, 1, 0);
                hit_record.material = m_Material;
                
                return true;
            }
        }

        return false;
    }

    AABB2Dzy::AABB2Dzy(const Vec3& min, double width, double height, const std::shared_ptr<Material>& material):
        AABB2D(min, width, height, material)
    {
    }

    bool AABB2Dzy::hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
    {
        const Vec3& min = m_Min;
        Vec3 max = Vec3(min.x, min.y + m_Height, min.z + m_Width);

        double t = (min.x - ray.origin().x) / ray.direction().x;

        if (t >= t_min && t <= t_max)
        {
            double z = ray.origin().z + t * ray.direction().z;
            double y = ray.origin().y + t * ray.direction().y;

            if (z >= min.z && z <= max.z && y >= min.y && y <= max.y)
            {
                hit_record.t = t;
                hit_record.p = ray.point(t);
                hit_record.normal = Vec3(1, 0, 0);
                hit_record.material = m_Material;
                
                return true;
            }
        }

        return false;
    }
}