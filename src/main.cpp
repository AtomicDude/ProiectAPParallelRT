#include "Drawable/Sphere/Sphere.h"
#include "Drawable/AABB/AABB3D.h"
#include "Material/DiffuseMaterial.h"
#include "Material/MetalMaterial.h"
#include "Material/DielectricMaterial.h"
#include "Image/Image.h"
#include "Camera/Camera.h"
#include "Scene/Scene.h"

#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
    const double ratio_w = 16.0;
    const double ratio_h = 9.0;
    const double ratio = ratio_w / ratio_h;
    uint32_t height = 100;

    if (argc >= 2)
    {
        height = std::stoi(argv[1]);
    }

    const uint32_t width = static_cast<uint32_t>(ratio * static_cast<double>(height));
    const uint32_t channels = 3;

    rt::Camera camera(
        rt::Vec3(0, 0, 0),         // eye
        rt::Vec3(0.0, 0.0, -1.0),  // lookAt
        rt::Vec3(0.0, 1.0, 0.0),   // up
        90.0,                      // fov
        ratio,                     // aspect ratio
        0.0                        // aperture
    );

    rt::Scene scene(width, height);
    scene.setBackgroundGradient(rt::Vec3(0.2, 0.4, 1.0), rt::Vec3(1.0, 1.0, 1.0));
    scene.add(std::make_shared<rt::Sphere>(
        rt::Vec3(0.0, -100.5, -1.0),
        100.0,
        std::make_shared<rt::DiffuseMaterial>(rt::Vec3(0.8, 0.8, 0.0))));
    scene.add(std::make_shared<rt::Sphere>(
        rt::Vec3(-1.0, 0.0, -1.0),
        0.5,
        std::make_shared<rt::DielectricMaterial>(rt::Vec3(1.0, 1.0, 1.0), 1.5)));
    scene.add(std::make_shared<rt::AABB3D>(
        rt::Vec3(-0.4, -0.4, -0.8),
        0.8,
        0.8,
        0.8,
        std::make_shared<rt::DiffuseMaterial>(rt::Vec3(0.9, 0.0, 0.2))));
    scene.add(std::make_shared<rt::Sphere>(
        rt::Vec3(1.0, 0.0, -1.0),
        0.5,
        std::make_shared<rt::MetalMaterial>(rt::Vec3(0.56, 0.57, 0.58), 0.01)));

    std::chrono::steady_clock clock;

    rt::Image image(width, height, channels);

    std::cout << "Rendering...\n";
    auto t0 = clock.now();
    scene.render(
        image,                          // outImage
        camera,                         // camera
        0,                              // x_start
        0,                              // y_start
        rt::Area(0, 0, width, height),  // imageArea
        64,                             // samples per pixel
        32,                             // max ray depth (bounces)
        1.5                             // gamma correction
    );
    
    auto t1 = clock.now();

    std::cout << "Done. ("  << std::chrono::duration<double>(t1 - t0).count() << "s)\n";

    std::cout << "Writing image...\n";
    t0 = clock.now();
    image.write("scene.png");
    t1 = clock.now();

    std::cout << "Done. (" << std::chrono::duration<double>(t1 - t0).count() << "s)\n";

    return 0;
}
