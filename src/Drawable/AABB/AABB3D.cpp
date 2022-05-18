#include "AABB3D.h"
#include "AABB2D.h"
#include <vector>
#include <math.h>

namespace rt
{
    AABB3D::AABB3D(const Vec3& min, double width, double height, double depth, const std::shared_ptr<Material>& material):
        Drawable(material),
        m_Faces
        {
            std::make_shared<AABB2Dxy>(Vec3(min.x, min.y, min.z), width, height, m_Material),                 // front
            std::make_shared<AABB2Dxy>(Vec3(min.x, min.y, min.z - depth), width, height, m_Material),         // back
            std::make_shared<AABB2Dzy>(Vec3(min.x, min.y, min.z - depth), depth, height, m_Material),         // left
            std::make_shared<AABB2Dzy>(Vec3(min.x + width, min.y, min.z - depth), depth, height, m_Material), // right
            std::make_shared<AABB2Dxz>(Vec3(min.x, min.y, min.z - depth), width, depth, m_Material),          // top
            std::make_shared<AABB2Dxz>(Vec3(min.x, min.y + height, min.z - depth), width, depth, m_Material)  // bottom
        }
    {
    }

    bool AABB3D::hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
    {
        HitRecord record;
        bool intersection = false;
        double closest_t = t_max;

        for (auto& face : m_Faces)
        {
            if (face->hit(ray, t_min, closest_t, record))
            {
                intersection = true;
                closest_t = record.t;
                hit_record = record;
            }
        }

        return intersection;
    }
}