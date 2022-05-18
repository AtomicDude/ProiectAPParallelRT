#include "Triangle.h"

#include <math.h>

#define EPS 1e-8

namespace rt
{
    Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c, const std::shared_ptr<Material>& material):
        Drawable(material),
        m_A(a),
        m_B(b),
        m_C(c)
    {
        m_N = Vec3::normalize(Vec3::cross(m_B - m_A, m_C - m_A));
    }

    bool Triangle::hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
    {   
        double dot_n_dir = Vec3::dot(m_N, ray.direction());
        if (fabs(dot_n_dir) < EPS)
        {
            return false;
        }

        double t = (Vec3::dot(m_N, m_A) - Vec3::dot(m_N, ray.origin())) / dot_n_dir;

        if (t >= t_min && t <= t_max)
        {
            Vec3 p = ray.point(t);
            Vec3 n;

            for (auto& [e0, e1] : std::initializer_list<std::tuple<Vec3, Vec3>>{ {m_A, m_B}, {m_B, m_C}, {m_C, m_A} })
            {
                Vec3 e0e1 = e1 - e0; 
                Vec3 e0p = p - e0;
                n = Vec3::cross(e0e1, e0p);

                if (Vec3::dot(m_N, n) < 0.0)
                {
                    return false;
                }
            }
        
            hit_record.t = t;
            hit_record.p = p;
            hit_record.normal = m_N;
            hit_record.material = m_Material;

            return true;
        }

        return false;
    }
}