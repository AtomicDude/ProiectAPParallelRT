#include "Image.h"

#include <random>

namespace rt
{
    Image::Image(int _width, int _height):
        width(_width),
        height(_height)
    {
    }

    void Image::setBackgroundGradient(const Vec3& _upColor, const Vec3& _downColor)
    {
        upColor = _upColor;
        downColor = _downColor;
    }

    void Image::emplaceSphere(const Vec3& c, float r)
    {
        drawableList.emplaceSphere(c, r);
    }

    void Image::render(const std::string& path, const Camera& camera, int antialiasing)
    {
        std::fstream fout(path, std::iostream::out);

        fout << "P3\n";
        fout << width << " " << height << "\n";
        fout << "255\n";

        for (int y = height - 1; y >= 0; y--)
        {
            for (int x = 0; x < width; x++)
            {
                Vec3 color(0.0f, 0.0f, 0.0f);

                for (int a = 0; a < antialiasing; a++)
                {
                    float u = ((float)x + (float)std::rand() / (float)RAND_MAX) / (float)width;
                    float v = ((float)y + (float)std::rand() / (float)RAND_MAX) / (float)height;

                    color += pixel(camera.castRay(u, v));
                }

                color /= (float)antialiasing;

                int r = (int)(255.0f * color.r);
                int g = (int)(255.0f * color.g);
                int b = (int)(255.0f * color.b);
                fout << r << " " << g << " " << b << "\n";
            }
        }
    }

    Vec3 Image::pixel(const Ray& ray) const
    {
        HitRecord rec;

        if (drawableList.hit(ray, 0.0f, 100000.0f, rec))
        {
            return 0.5f * Vec3(rec.normal.x + 1.0f, rec.normal.y + 1.0f, rec.normal.z + 1.0f);
        }

        // background
        Vec3 d = normalize(ray.direction());
        float t = (d.y + 1.0f) / 2.0f;

        return lerp(t, upColor, downColor);
    }
}