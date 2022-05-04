#include "Sphere.h"

namespace rt
{
    Sphere::Sphere(Vec3 c, double r, const std::shared_ptr<Material>& material):
        Drawable(material),
        m_Center(c),
        m_Radius(r)
    {
    }

    bool Sphere::hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
    {
        Vec3 co = ray.origin() - m_Center;
        double a = Vec3::dot(ray.direction(), ray.direction());
        double b = 2.0 * Vec3::dot(co, ray.direction());
        double c = Vec3::dot(co, co) - m_Radius * m_Radius;
        double delta = b * b - 4.0 * a * c;

        if (delta >= 0.0)
        {
            double sqrt_delta = sqrt(delta);
            for (double t : { (-b - sqrt_delta) / (2.0 * a), (-b + sqrt_delta) / (2.0 * a) })
            {
                if (t >= t_min && t <= t_max)
                {
                    hit_record.t = t;
                    hit_record.p = ray.point(t);
                    hit_record.normal = (hit_record.p - m_Center) / m_Radius;
                    hit_record.material = m_Material;

                    return true;
                }
            }
        }

        return false;
    }

    const Vec3& Sphere::center() const
    {
        return m_Center;
    }

    double Sphere::radius() const
    {
        return m_Radius;
    }
}