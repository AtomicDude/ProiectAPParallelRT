#include "AABB3D.h"
#include "../Triangle/Triangle.h"
#include <math.h>

namespace rt
{
    AABB3D::AABB3D(const Vec3& min, double width, double height, double depth, const std::shared_ptr<Material>& material):
        Drawable(material),
        m_Min(min),
        m_Width(width),
        m_Height(height),
        m_Depth(depth)
    {
    }

    bool AABB3D::hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
    {
        const Vec3& a = m_Min;
        Vec3 b = Vec3(a.x + m_Width, a.y, a.z);
        Vec3 c = Vec3(a.x + m_Width, a.y, a.z + m_Depth);
        Vec3 d = Vec3(a.x, a.y, a.z + m_Depth);
        Vec3 e = Vec3(a.x, a.y + m_Height, a.z);
        Vec3 f = Vec3(a.x + m_Width, a.y + m_Height, a.z);
        Vec3 g = Vec3(a.x + m_Width, a.y + m_Height, a.z + m_Depth);
        Vec3 h = Vec3(a.x, a.y + m_Height, a.z + m_Depth);

        Triangle triangles[] =
        {
            Triangle(a, b, f, m_Material),
            Triangle(f, e, a, m_Material),
            Triangle(b, c, g, m_Material),
            Triangle(g, f, b, m_Material),
            Triangle(c, d, h, m_Material),
            Triangle(h, g, c, m_Material),
            Triangle(d, a, e, m_Material),
            Triangle(e, h, d, m_Material),
            Triangle(d, a, b, m_Material),
            Triangle(b, c, d, m_Material),
            Triangle(e, f, g, m_Material),
            Triangle(g, h, e, m_Material),
        };

        HitRecord record;
        bool intersection = false;
        double closest_t = t_max;

        for (auto& triangle : triangles)
        {
            if (triangle.hit(ray, t_min, closest_t, record))
            {
                intersection = true;
                closest_t = record.t;
                hit_record = record;
            }
        }

        return intersection;
    }
}