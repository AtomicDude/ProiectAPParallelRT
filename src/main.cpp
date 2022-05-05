#include "Drawable/Sphere/Sphere.h"
#include "Material/DiffuseMaterial.h"
#include "Material/MetalMaterial.h"
#include "Material/DielectricMaterial.h"

#include "Camera/Camera.h"
#include "Scene/Scene.h"
#include <iostream>
#include <chrono>

int main()
{
    uint32_t factor = 40;
    uint32_t width = 16 * factor;
    uint32_t height = 9 * factor;
    
    rt::Camera camera(
        rt::Vec3(0, 0, 0),                                          // eye
        rt::Vec3(0.0, 0.0, -1.0),                                   // lookAt
        rt::Vec3(0.0, 1.0, 0.0),                                    // up
        90.0,                                                       // fov
        static_cast<double>(width) / static_cast<double>(height),   // aspect ratio
        0.0                                                         // aperture
    );

    rt::Scene scene(width, height);
    scene.setBackgroundGradient(rt::Vec3(0.2, 0.4, 1.0), rt::Vec3(1.0, 1.0, 1.0));
    scene.add(std::make_shared<rt::Sphere>(
        rt::Vec3(0.0, -100.5, -1.0),
        100.0,
        std::make_shared<rt::DiffuseMaterial>(rt::Vec3(0.8, 0.8, 0.0))
    ));
    scene.add(std::make_shared<rt::Sphere>(
        rt::Vec3(-1.0, 0.0, -1.0),
        0.5,
        std::make_shared<rt::DielectricMaterial>(rt::Vec3(1.0, 1.0, 1.0), 1.5)
    ));
    scene.add(std::make_shared<rt::Sphere>(
        rt::Vec3(0.0, 0.0, -1.0),
        0.5,
        std::make_shared<rt::DiffuseMaterial>(rt::Vec3(0.8, 0.6, 0.2))
    ));
    scene.add(std::make_shared<rt::Sphere>(
        rt::Vec3(1.0, 0.0, -1.0),
        0.5,
        std::make_shared<rt::MetalMaterial>(rt::Vec3(0.56, 0.57, 0.58), 0.01)
    ));

    std::chrono::steady_clock clock;

    std::cout << "Rendering...\n";
    auto t0 = clock.now();
    scene.render(camera, 64, 32, 1.5);
    auto t1 = clock.now();

    std::cout << "Done. ("  << std::chrono::duration<double, std::milli>(t1 - t0).count() << "ms)\n";

    std::cout << "Writing image...\n";
    t0 = clock.now();
    scene.writePNG("spheres.png");
    t1 = clock.now();

    std::cout << "Done. (" << std::chrono::duration<double, std::milli>(t1 - t0).count() << "ms)\n";

    return 0;
}
