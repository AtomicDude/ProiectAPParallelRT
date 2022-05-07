#include "Scene.h"

#include "../Utility/Utility.h"
#include "../stb_image/stb_image_write.h"

namespace rt
{
    Scene::Scene(uint32_t width, uint32_t height, uint32_t channels):
        m_Width(width),
        m_Height(height),
        m_Channels(channels),
        m_ImageData(width * height * channels, 255)
    {
    }

    void Scene::setBackgroundGradient(const Vec3& downColor, const Vec3& upColor)
    {
        m_DownColor = downColor;
        m_UpColor = upColor;
    }

    void Scene::add(const std::shared_ptr<Drawable>& drawable)
    {
        m_Drawables.push_back(drawable);
    }

    void Scene::render(
            const Camera& camera,
            uint32_t x_start,
            uint32_t y_start,
            uint32_t width,
            uint32_t height,
            uint32_t samples,
            uint32_t depth,
            double gamma
        )
    {
        for (uint32_t y = y_start; y < y_start + height; y++)
        {
            for (uint32_t x = x_start; x < x_start + width; x++)
            {
                Vec3 color(0.0, 0.0, 0.0);

                for (uint32_t s = 0; s < samples; s++)
                {
                    double u = (static_cast<double>(x) + random_double()) / static_cast<double>(m_Width);
                    double v = (static_cast<double>(y) + random_double()) / static_cast<double>(m_Height);

                    color += computeColor(camera.castRay(u, v), depth);
                }

                color /= static_cast<double>(samples);

                if (gamma > 0.0)
                {
                    double inv_gamma = 1.0 / gamma;

                    color = Vec3(pow(color.x, inv_gamma), pow(color.y, inv_gamma), pow(color.z, inv_gamma));
                }

                uint32_t index = (m_Height - 1 - y) * m_Width * m_Channels + x * m_Channels;

                m_ImageData[index + 0] = static_cast<uint8_t>(255.0 * color.r);
                m_ImageData[index + 1] = static_cast<uint8_t>(255.0 * color.g);
                m_ImageData[index + 2] = static_cast<uint8_t>(255.0 * color.b);
            }
        }
    }

    void Scene::writePNG(const std::string& path) const
    {
        stbi_write_png(
            path.c_str(),
            static_cast<int>(m_Width),
            static_cast<int>(m_Height),
            static_cast<int>(m_Channels),
            m_ImageData.data(),
            static_cast<int>(m_Width * m_Channels)
        );
    }

    std::vector<uint8_t>& Scene::imageVector()
    {
        return m_ImageData;
    }

    bool Scene::hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
    {
        HitRecord record;
        bool intersection = false;
        double closest_t = t_max;

        for (auto& drawable : m_Drawables)
        {
            if (drawable->hit(ray, t_min, closest_t, record))
            {
                intersection = true;
                closest_t = record.t;
                hit_record = record;
            }
        }

        return intersection;
    }

    Vec3 Scene::computeColor(const Ray& ray, uint32_t depth) const
    {
        if (depth == 0)
        {
            return Vec3(0.0, 0.0, 0.0);
        }

        HitRecord hit_record;

        if (hit(ray, 1e-7, 1e+7, hit_record))
        {
            Ray outRay;
            Vec3 outColor;
            
            bool isOutside = Vec3::dot(ray.direction(), hit_record.normal) < 0.0;
            hit_record.normal = isOutside ? hit_record.normal : -hit_record.normal;

            if (hit_record.material->scatter(ray, hit_record.p, hit_record.normal, isOutside, outColor, outRay))
            {
                return outColor * computeColor(outRay, depth - 1);
            }

            return Vec3(0.0, 0.0, 0.0);
        }

        // background
        Vec3 d = Vec3::normalize(ray.direction());
        double t = (d.y + 1.0) / 2.0;

        return Vec3::lerp(t, m_UpColor, m_DownColor);
    }
}