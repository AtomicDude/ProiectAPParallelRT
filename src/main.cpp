#include "raytracing/raytracing.h"
#include <iostream>
#include <fstream>

int main()
{
    int width = 200;
    int height = 100;

    rt::vec3 eye(0.0f, 0.0f, 0.0f);

    rt::vec3 lower_left(-2.0f, -1.0f, -1.0f);
    rt::vec3 horizontal(4.0f, 0.0f, 0.0f);
    rt::vec3 vertical(0.0f, 2.0f, 0.0f);

    rt::vec3 start_color(0.5f, 0.7f, 1.0f);
    rt::vec3 end_color(0.0f, 0.8f, 0.2f);

    std::vector<std::vector<rt::vec3>> pixels(height, std::vector<rt::vec3>(width));

    
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            float u = (float)x / (float)width;
            float v = (float)y / (float)height;

            rt::vec3 direction = lower_left + u * horizontal + v * vertical;

            rt::ray ray(eye, direction);

            rt::vec3 color = rt::gradient_color(ray, start_color, end_color);

            pixels[y][x] = color;
        }
    }

    rt::write_image("gradient.ppm", pixels);

    return 0;
}
