#include "Drawable/Sphere/Sphere.h"
#include "Drawable/AABB/AABB3D.h"
#include "Material/DiffuseMaterial.h"
#include "Material/MetalMaterial.h"
#include "Material/DielectricMaterial.h"
#include "Camera/Camera.h"
#include "Area/Area.h"
#include "Scene/Scene.h"

#include <iostream>
#include <chrono>
#include <mpi.h>

int main(int argc, char** argv)
{
    const double ratio_w = 16.0;
    const double ratio_h = 9.0;
    const double ratio = ratio_w / ratio_h;
    uint32_t height = 100;
    bool useBorder = false;

    if (argc == 2)
    {
        height = std::stoi(argv[1]);
    }

    if (argc >= 3)
    {
        height = std::stoi(argv[1]);
        useBorder = true;
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
                globalImage,        // outImage
                camera,             // camera
                x_start,            // x_start
                y_start,            // y_start
                imageArea,          // imageArea
                64,                 // samples per pixel
                32,                 // max ray depth (bounces)
                1.5,                // gamma correction
                useBorder
            );
        }

        auto t1 = clock.now();
        std::cout << "Done. ("  << std::chrono::duration<double>(t1 - t0).count() << "s)\n";

        std::cout << "Writing image...\n";
        t0 = clock.now();
        globalImage.write("scene.png");
        t1 = clock.now();

        std::cout << "Done. (" << std::chrono::duration<double>(t1 - t0).count() << "s)\n";
    }

    MPI_Finalize();

    return 0;
}
