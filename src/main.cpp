#include "Drawable/Sphere/Sphere.h"
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
#include <mpi.h>

int main(int argc, char** argv)
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
    bool useBorder = sceneParams.params.border;
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
    std::chrono::steady_clock::time_point t0;

    MPI_Init(&argc, &argv);
    
    int32_t comm_size;
    int32_t comm_rank;
    const int32_t root = 0;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_size(comm, &comm_size);
    MPI_Comm_rank(comm, &comm_rank);

    uint32_t pixelRows = height / comm_size;
    uint32_t remainingRows = height % comm_size;

    uint32_t x_start = 0;
    uint32_t y_start = (comm_size - comm_rank - 1) * pixelRows;

    rt::Area imageArea;
    imageArea.x = 0;
    imageArea.y = 0;
    imageArea.width = width;
    imageArea.height = pixelRows;

    rt::Image globalImage(channels);
    rt::Image localImage(imageArea.width, imageArea.height, channels);

    if (comm_rank == root)
    {
        globalImage.resize(width, height);

        std::cout << "Rendering...\n" << std::flush;
        t0 = clock.now();
    }

    MPI_Barrier(comm); // wait for root to output initialize

    std::cout << "Start " << comm_rank << "\n";

    scene.render(
        localImage,         // outImage
        camera,             // camera
        x_start,            // x_start
        y_start,            // y_start
        imageArea,          // imageArea
        64,                 // samples per pixel
        32,                 // max ray depth (bounces)
        1.5,                // gamma correction
        useBorder
    );

    std::cout << "Stop " << comm_rank << "\n";

    MPI_Gather(
        localImage.data(),
        static_cast<int>(localImage.size()),
        MPI_BYTE,
        globalImage.data(),
        static_cast<int>(localImage.size()),
        MPI_BYTE,
        root,
        comm
    );

    if (comm_rank == root)
    {
        if (remainingRows > 0)
        {
            x_start = 0;
            y_start = 0;

            imageArea.x = 0;
            imageArea.y = height - remainingRows;
            imageArea.width = width;
            imageArea.height = remainingRows;

            // render the remaining rows
            scene.render(
                globalImage,                 // outImage
                camera,                      // camera
                x_start,                     // x_start
                y_start,                     // y_start
                imageArea,                   // imageArea
                sceneParams.params.samples,  // samples per pixel
                sceneParams.params.bounces,  // max ray depth (bounces)
                sceneParams.params.gamma,    // gamma correction
                useBorder
            );
        }

        auto t1 = clock.now();
        std::cout << "Done. ("  << std::chrono::duration<double>(t1 - t0).count() << "s)\n";

        std::cout << "Writing image...\n";
        t0 = clock.now();
        globalImage.write(sceneParams.title + ".png");
        t1 = clock.now();

        std::cout << "Done. (" << std::chrono::duration<double>(t1 - t0).count() << "s)\n";
    }

    MPI_Finalize();

    return 0;
}
