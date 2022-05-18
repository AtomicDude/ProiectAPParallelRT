#pragma once

#include "../Drawable/Sphere/Sphere.h"
#include "../Camera/Camera.h"
#include "../Image/Image.h"
#include "../Area/Area.h"

namespace rt
{
    class Scene
    {
    public:
        Scene(uint32_t width, uint32_t height);
        void setBackgroundGradient(const Vec3& downColor, const Vec3& upColor);
        void add(const std::shared_ptr<Drawable>& drawable);
        void render(
            Image& image,
            const Camera& camera,
            uint32_t x_start,
            uint32_t y_start,
            const Area& imageArea,
            uint32_t samples = 64,
            uint32_t depth = 32,
            double gamma = 1.0,
            bool useBorder = false
        );

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