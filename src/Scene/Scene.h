#pragma once

#include "../Drawable/Sphere/Sphere.h"
#include "../Camera/Camera.h"

#include <string>
#include <vector>

namespace rt
{
    class Scene
    {
    public:
        Scene(uint32_t width, uint32_t height, uint32_t channels = 3);
        void setBackgroundGradient(const Vec3& downColor, const Vec3& upColor);
        void add(const std::shared_ptr<Drawable>& drawable);
        void render(
            const Camera& camera,
            uint32_t x_start,
            uint32_t y_start,
            uint32_t width,
            uint32_t height,
            uint32_t samples = 64,
            uint32_t depth = 32,
            double gamma = 1.0
        );
        void writePNG(const std::string& path) const;
        std::vector<uint8_t>& imageVector();

    private:
        bool hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const;
        Vec3 computeColor(const Ray& ray, uint32_t depth) const;

    private:
        uint32_t m_Width;
        uint32_t m_Height;
        uint32_t m_Channels;
        Vec3 m_DownColor;
        Vec3 m_UpColor;
        std::vector<std::shared_ptr<Drawable>> m_Drawables;
        std::vector<uint8_t> m_ImageData;
    };
}