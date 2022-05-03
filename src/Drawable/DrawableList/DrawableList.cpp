#include "DrawableList.h"

namespace rt
{
    void DrawableList::pushSphere(const Sphere& sphere)
    {
        drawables.push_back(new Sphere(sphere.center(), sphere.radius()));
    }

    void DrawableList::emplaceSphere(const Vec3& c, float r)
    {
        drawables.push_back(new Sphere(c, r));
    }

    bool DrawableList::hit(const Ray& Ray, float t_min, float t_max, HitRecord& hit_record) const
    {
        HitRecord rec;
        bool hit = false;
        float closest_t = t_max;

        for (auto drawable : drawables)
        {
            if (drawable->hit(Ray, t_min, closest_t, rec))
            {
                hit = true;
                closest_t = rec.t;
                hit_record = rec;
            }
        }

        return hit;
    }
}