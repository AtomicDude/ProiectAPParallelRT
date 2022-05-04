#pragma once

#include "../Drawable/Sphere/Sphere.h"
#include "../Camera/Camera.h"

#include <string>
#include <vector>

namespace rt
{
    class Image
    {
    public:
        Image(uint32_t width, uint32_t height);
        void setBackgroundGradient(const Vec3& downColor, const Vec3& upColor);
        void emplaceSphere(const Vec3& c, double r, const std::shared_ptr<Material>& material);
        void render(const std::string& path, const Camera& camera, uint32_t samples = 64, uint32_t depth = 32);

    private:
        bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const;
        Vec3 computeColor(const Ray& ray, uint32_t depth) const;

    private:
        uint32_t m_Width;
        uint32_t m_Height;
        Vec3 m_DownColor;
        Vec3 m_UpColor;
        std::vector<std::shared_ptr<Drawable>> m_Drawables;
    };
}