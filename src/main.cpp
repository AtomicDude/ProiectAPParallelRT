#include "Drawable/Sphere/Sphere.h"
#include "Material/DiffuseMaterial.h"
#include "Material/MetalMaterial.h"
#include "Material/DielectricMaterial.h"

#include "Camera/Camera.h"
#include "Scene/Scene.h"
#include <iostream>
#include <chrono>

#include <mpi.h>

struct PixelArea
{
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
};

int main(int argc, char** argv)
{
    const double ratio_w = 16.0;
    const double ratio_h = 9.0;
    const double ratio = ratio_w / ratio_h;
    const uint32_t height = 720;
    const uint32_t width = static_cast<uint32_t>(ratio * static_cast<double>(height));
    const uint32_t channels = 3;

    rt::Camera camera(
        rt::Vec3(0, 0, 0),                                          // eye
        rt::Vec3(0.0, 0.0, -1.0),                                   // lookAt
        rt::Vec3(0.0, 1.0, 0.0),                                    // up
        90.0,                                                       // fov
        static_cast<double>(width) / static_cast<double>(height),   // aspect ratio
        0.0                                                         // aperture
    );
    
    rt::Scene scene(width, height, channels);
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

    PixelArea pixelArea;
    pixelArea.x = 0;
    pixelArea.y = height - (comm_rank + 1) * pixelRows;
    pixelArea.width = width;
    pixelArea.height = pixelRows;

    if (comm_rank == root)
    {
        std::cout << "Rendering...\n" << std::flush;
        t0 = clock.now();
    }

    MPI_Barrier(comm); // wait for root to output initialize

    std::cout << "Start " << comm_rank << "\n";

    scene.render(
        camera,             // camera
        pixelArea.x,        // x_start
        pixelArea.y,        // y_start
        pixelArea.width,    // width
        pixelArea.height,   // height
        64,                 // samples per pixel
        32,                 // max ray depth (bounces)
        1.5                 // gamma correction
    );

    std::cout << "Stop " << comm_rank << "\n";

    std::vector<uint8_t>& imageVector = scene.imageVector();
    const size_t offset = (height - pixelArea.y - pixelArea.height) * pixelArea.width * channels + pixelArea.x * channels;
    const size_t send_size = pixelArea.width * pixelArea.height * channels;

    MPI_Gather(
        imageVector.data() + offset,
        static_cast<int>(send_size),
        MPI_BYTE,
        imageVector.data(),
        static_cast<int>(send_size),
        MPI_BYTE,
        root,
        comm
    );

    if (comm_rank == root)
    {
        if (remainingRows > 0)
        {
            pixelArea.x = 0;
            pixelArea.y = 0;
            pixelArea.width = width;
            pixelArea.height = remainingRows;

            // render the remaining rows
            scene.render(
                camera,             // camera
                pixelArea.x,        // x_start
                pixelArea.y,        // y_start
                pixelArea.width,    // width
                pixelArea.height,   // height
                64,                 // samples per pixel
                32,                 // max ray depth (bounces)
                1.5                 // gamma correction
            );
        }

        auto t1 = clock.now();
        std::cout << "Done. ("  << std::chrono::duration<double>(t1 - t0).count() << "s)\n";

        std::cout << "Writing image...\n";
        t0 = clock.now();
        scene.writePNG("spheres.png");
        t1 = clock.now();

        std::cout << "Done. (" << std::chrono::duration<double>(t1 - t0).count() << "s)\n";
    }

    MPI_Finalize();

    return 0;
}
