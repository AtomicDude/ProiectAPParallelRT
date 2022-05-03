#pragma once

#include "../Drawable.h"

namespace rt
{
    class Sphere : public Drawable
    {
    public:
        Sphere(Vec3 _c = Vec3(0.0f, 0.0f, 0.0f), float _r = 1.0f);
        virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override;

        const Vec3& center() const;
        float radius() const;

    private:
        Vec3 c;
        float r;
    };
}