#include "Sphere.h"

namespace rt
{
    Sphere::Sphere(Vec3 _c, float _r):
        c(_c),
        r(_r)
    {
    }

    bool Sphere::hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const
    {
        Vec3 co = ray.origin() - c;
        float a = dot(ray.direction(), ray.direction());
        float b = 2.0f * dot(co, ray.direction());
        float c = dot(co, co) - r * r;
        float delta = b * b - 4.0f * a * c;

        if (delta > 0)
        {
            float t;
            t = (-b - sqrtf(delta)) / (2.0f * a);

            if (t >= t_min && t <= t_max)
            {
                hit_record.t = t;
                hit_record.p = ray.point(t);
                hit_record.normal = (hit_record.p - c) / r;

                return true;
            }

            t = (-b + sqrtf(delta)) / (2.0f * a);

            if (t >= t_min && t <= t_max)
            {
                hit_record.t = t;
                hit_record.p = ray.point(t);
                hit_record.normal = (hit_record.p - c) / r;

                return true;
            }
        }

        return false;
    }

    const Vec3& Sphere::center() const
    {
        return c;
    }

    float Sphere::radius() const
    {
        return r;
    }
}