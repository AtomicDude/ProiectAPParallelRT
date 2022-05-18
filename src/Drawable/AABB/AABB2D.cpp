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

    bool AABB2D::hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
    {
        const Vec3& a = m_Min;
        Vec3 b = Vec3(a.x + m_Width, a.y, a.z);
        Vec3 c = Vec3(a.x + m_Width, a.y + m_Height, a.z);
        Vec3 d = Vec3(a.x, m_Min.y + m_Height, a.z);
        
        Triangle triangles[] = 
        {
            Triangle(a, b, c, m_Material),
            Triangle(c, d, a, m_Material)
        };

        for (auto& triangle : triangles)
        {
            if (triangle.hit(ray, t_min, t_max, hit_record))
            {
                return true;
            }
        }

        return false;
    }
}