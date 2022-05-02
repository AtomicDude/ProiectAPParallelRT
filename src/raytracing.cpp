#include "raytracing.h"

namespace rt
{
    vec3 gradient_color(const ray& ray, const vec3& color1, const vec3& color2)
    {
        vec3 d = normalize(ray.direction());
        float t = (d.y + 1.0f) / 2.0f;

        return lerp(t, color1, color2);
    }

    void write_image(const std::string file_name, const std::vector<std::vector<vec3>>& pixels)
    {
        std::fstream fout(file_name, std::iostream::out);

        size_t width = pixels.back().size();
        size_t height = pixels.size();

        fout << "P3\n";
        fout << width << " " << height << "\n";
        fout << "255\n";

        for (int y = height - 1; y >= 0; y--)
        {
            for (int x = 0; x < width; x++)
            {
                int ir = (int)(255.0f * pixels[y][x].r);
                int ig = (int)(255.0f * pixels[y][x].g);
                int ib = (int)(255.0f * pixels[y][x].b);
                fout << ir << " " << ig << " " << ib << "\n";
            }
        }
    }
}