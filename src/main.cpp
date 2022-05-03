#include "Drawable/Sphere/Sphere.h"
#include "Camera/Camera.h"
#include "Image/Image.h"

#include <iostream>
#include <chrono>

int main()
{
    int factor = 80;
    int width = 16 * factor;
    int height = 9 * factor;
    
    rt::Camera camera(
        rt::Vec3(0.0f, 0.0f, 0.0f),  // eye
        rt::Vec3(0.0f, 0.0f, -1.0f), // lookat
        rt::Vec3(0.0f, 1.0f, 0.0f),  // up
        90.0f,                       // fov
        (float)width / (float)height // aspect ratio
    );

    rt::Image image(width, height);
    image.setBackgroundGradient(rt::Vec3(0.5f, 0.7f, 1.0f), rt::Vec3(0.0f, 0.8f, 0.2f));
    image.emplaceSphere(rt::Vec3(-2.0f, 0.0f, -3.0f), 1.0f);
    image.emplaceSphere(rt::Vec3(2.0f, 0.0f, -3.0f), 1.0f);
    image.emplaceSphere(rt::Vec3(0.0f, -101.0f, -1.0f), 100.0f);

    std::chrono::steady_clock clock;

    std::cout << "Rendering...\n";
    auto t0 = clock.now();
    image.render("spheres.ppm", camera, 32);
    auto t1 = clock.now();

    std::cout << "Done. ("  << std::chrono::duration<float, std::milli>(t1 - t0).count() << "ms)";

    return 0;
}
