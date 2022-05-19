#include "Drawable/Sphere/Sphere.h"
#include "Drawable/Triangle/Triangle.h"
#include "Drawable/AABB/AABB2D.h"
#include "Drawable/AABB/AABB3D.h"
#include "Material/DiffuseMaterial.h"
#include "Material/MetalMaterial.h"
#include "Material/DielectricMaterial.h"

#include "Camera/Camera.h"
#include "Area/Area.h"
#include "Scene/Scene.h"

#include "json/Serialization/SceneSerialization.h"

#include <iostream>
#include <fstream>
#include <chrono>

using json = nlohmann::json;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "One json file is required.\n";
        return 0;
    }

    std::string scenePath = argv[1];

    std::fstream file(scenePath, std::ios::in);
    if (!file)
    {
        std::cout << "Could not open file "
                  << "\"" << scenePath << "\"\n";
        return 0;
    }

    json j;
    file >> j;
    file.close();

    rt::SceneSerialization sceneParams;
    j.get_to(sceneParams);

    const double ratio_w = 16.0;
    const double ratio_h = 9.0;
    const double ratio = ratio_w / ratio_h;
    uint32_t height = sceneParams.params.height;
    const uint32_t width = static_cast<uint32_t>(ratio * static_cast<double>(height));
    const uint32_t channels = 3;

    rt::CameraSerialization &cameraParams = sceneParams.params.camera;

    rt::Camera camera(
        cameraParams.position, // eye
        cameraParams.lookAt,   // lookAt
        cameraParams.up,       // up
        cameraParams.fovy,     // fov
        ratio,                 // aspect ratio
        cameraParams.aperture  // aperture
    );

    rt::Scene scene(width, height);
    scene.setBackgroundGradient(rt::Vec3(0.2, 0.4, 1.0), rt::Vec3(1.0, 1.0, 1.0));

    for (rt::DrawableSerialization &drawableParams : sceneParams.drawables)
    {
        rt::MaterialSerialization &materialParams = drawableParams.params.material;
        std::shared_ptr<rt::Material> material;

        if (materialParams.type == "diffuse")
        {
            material = std::make_shared<rt::DiffuseMaterial>(materialParams.params.color);
        }
        else if (materialParams.type == "metal")
        {
            material = std::make_shared<rt::MetalMaterial>(materialParams.params.color, materialParams.params.fuzziness);
        }
        else if (materialParams.type == "dielectric")
        {
            material = std::make_shared<rt::DielectricMaterial>(materialParams.params.color, materialParams.params.refraction);
        }

        if (drawableParams.type == "sphere")
        {
            scene.add(std::make_shared<rt::Sphere>(
                drawableParams.params.center,
                drawableParams.params.radius,
                material));
        }
        else if (drawableParams.type == "aabb3d")
        {
            scene.add(std::make_shared<rt::AABB3D>(
                drawableParams.params.position,
                drawableParams.params.width,
                drawableParams.params.height,
                drawableParams.params.depth,
                material));
        }
    }

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
        sceneParams.params.samples,     // samples per pixel
        sceneParams.params.bounces,     // max ray depth (bounces)
        sceneParams.params.gamma        // gamma correction
    );
    
    auto t1 = clock.now();

    std::cout << "Done. ("  << std::chrono::duration<double>(t1 - t0).count() << "s)\n";

    std::cout << "Writing image...\n";
    t0 = clock.now();
    image.write(sceneParams.title + ".png");
    t1 = clock.now();

    std::cout << "Done. (" << std::chrono::duration<double>(t1 - t0).count() << "s)\n";

    return 0;
}
