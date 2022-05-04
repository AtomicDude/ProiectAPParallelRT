#include "Scene.h"

#include "../Utility/Utility.h"
#include <fstream>

namespace rt
{
    Scene::Scene(uint32_t width, uint32_t height):
        m_Width(width),
        m_Height(height)
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

    void Scene::render(const std::string& path, const Camera& camera, uint32_t samples, uint32_t depth)
    {
        std::vector<Vec3> pixels(static_cast<size_t>(m_Width) * static_cast<size_t>(m_Height));

        std::fstream fout(path, std::iostream::out);

        fout << "P3\n";
        fout << m_Width << " " << m_Height << "\n";
        fout << "255\n";

        for (uint32_t y = 0; y < m_Height; y++)
        {
            for (uint32_t x = 0; x < m_Width; x++)
            {
                Vec3 color(0.0, 0.0, 0.0);

                for (uint32_t s = 0; s < samples; s++)
                {
                    double u = (static_cast<double>(x) + random_double()) / static_cast<double>(m_Width);
                    double v = (static_cast<double>(y) + random_double()) / static_cast<double>(m_Height);

                    color += computeColor(camera.castRay(u, v), depth);
                }

                color /= static_cast<double>(samples);
                color = Vec3(sqrt(color.x), sqrt(color.y), sqrt(color.z));
                pixels[y * m_Width + x] = color;
            }
        }

        for (uint32_t y = 0; y < m_Height; y++)
        {
            for (uint32_t x = 0; x < m_Width; x++)
            {
                Vec3& pixel = pixels[(m_Height - 1 - y) * m_Width + x];

                uint32_t r = static_cast<uint32_t>(255.0 * pixel.r);
                uint32_t g = static_cast<uint32_t>(255.0 * pixel.g);
                uint32_t b = static_cast<uint32_t>(255.0 * pixel.b);
                fout << r << " " << g << " " << b << "\n";
            }
        }
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