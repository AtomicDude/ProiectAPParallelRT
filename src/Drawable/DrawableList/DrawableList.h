#pragma once

#include "../Drawable.h"
#include "../Sphere/Sphere.h"
#include <vector>

namespace rt
{
    class DrawableList : public Drawable
    {
    public:
        void pushSphere(const Sphere& sphere);
        void emplaceSphere(const Vec3& c, float r);
        virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override;

    private:
        std::vector<Drawable*> drawables;
    };
}