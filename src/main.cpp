#include "Drawable/Sphere/Sphere.h"
#include "Material/DiffuseMaterial.h"
#include "Material/MetalMaterial.h"
#include "Material/DielectricMaterial.h"

#include "Camera/Camera.h"
#include "Area/Area.h"
#include "Scene/Scene.h"

#include <iostream>
#include <chrono>
#include <mpi/mpi.h>

int main(int argc, char **argv)
{
    const double ratio_w = 16.0;
    const double ratio_h = 9.0;
    const double ratio = ratio_w / ratio_h;
    const uint32_t height = 720;
    const uint32_t width = static_cast<uint32_t>(ratio * static_cast<double>(height));
    const uint32_t channels = 3;
    bool useBorder = true;
    const uint32_t width_granularity = 2;
    const uint32_t height_granularity = 2;

    rt::Camera camera(
        rt::Vec3(0, 0, 0),                                        // eye
        rt::Vec3(0.0, 0.0, -1.0),                                 // lookAt
        rt::Vec3(0.0, 1.0, 0.0),                                  // up
        90.0,                                                     // fov
        static_cast<double>(width) / static_cast<double>(height), // aspect ratio
        0.0                                                       // aperture
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
    scene.add(std::make_shared<rt::Sphere>(
        rt::Vec3(0.0, 0.0, -1.0),
        0.5,
        std::make_shared<rt::DiffuseMaterial>(rt::Vec3(0.8, 0.6, 0.2))));
    scene.add(std::make_shared<rt::Sphere>(
        rt::Vec3(1.0, 0.0, -1.0),
        0.5,
        std::make_shared<rt::MetalMaterial>(rt::Vec3(0.56, 0.57, 0.58), 0.01)));

    std::chrono::steady_clock clock;
    std::chrono::steady_clock::time_point t0;

    std::vector<rt::Area> renderAreas;

    MPI_Init(&argc, &argv);

    int32_t comm_size;
    int32_t comm_rank;
    const int32_t root = 0;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_size(comm, &comm_size);
    MPI_Comm_rank(comm, &comm_rank);

    const int32_t default_area_height = static_cast<int32_t>(height / (comm_size * width_granularity));
    const int32_t default_area_width = static_cast<int32_t>(width / (comm_size * height_granularity));

    for (uint32_t y = 0; y < height ; y += default_area_height)
    {
        for (uint32_t x = 0; x < width; x += default_area_width)
        {
            renderAreas.push_back(
                rt::Area(
                    x,
                    y,
                    (x + default_area_width) < width ? default_area_width : (width - x),
                    (y + default_area_height) < height ? default_area_height : (height - y)
                )
            );
        }
    }

    rt::Image globalImage(channels);
    std::vector<MPI_Request> requests;

    if (comm_rank == root)
    {
        globalImage.resize(width, height);
        requests.resize(renderAreas.size());

        std::cout << "Rendering..." << std::endl;
        t0 = clock.now();
    }

    MPI_Barrier(comm); // wait for root to output initialize
    
    if (comm_rank == root)
    {
        for (uint32_t i = 0; i < renderAreas.size(); i++)
        {
            rt::Area& area = renderAreas[i];

            MPI_Datatype blockType;
            MPI_Type_vector(area.height, area.width * channels, width * channels, MPI_BYTE, &blockType);
            MPI_Type_commit(&blockType);

            int32_t y = static_cast<int32_t>(height) - static_cast<int32_t>(area.y) - static_cast<int32_t>(default_area_height);

            if (y < 0)
            {
                y = 0;
            }

            uint32_t offset = static_cast<uint32_t>(y) * width * channels + area.x * channels;
            
            MPI_Irecv(globalImage.data() + offset, 1, blockType, i % comm_size, i, comm, &requests[i]);
        }
    }

    for (uint32_t i = comm_rank; i < renderAreas.size(); i += comm_size)
    {
        std::cout << "Start " << comm_rank << "\n";
        rt::Area& area = renderAreas[i];

        rt::Image localImage(area.width, area.height, channels);

        scene.render(
            localImage,                              // outImage
            camera,                                  // camera
            area.x,                                  // x_start
            area.y,                                  // y_start
            rt::Area(0, 0, area.width, area.height), // imageArea
            64,                                      // samples per pixel
            32,                                      // max ray depth (bounces)
            1.5,                                     // gamma correction
            useBorder                                // border
        );

        std::cout << "Stop " << comm_rank << "\n";
        MPI_Send(localImage.data(), localImage.size(), MPI_BYTE, root, i, comm);
    }

    if (comm_rank == root)
    {
        MPI_Waitall(requests.size(), requests.data(), MPI_STATUSES_IGNORE);

        auto t1 = clock.now();
        std::cout << "Done. (" << std::chrono::duration<double>(t1 - t0).count() << "s)\n";

        std::cout << "Writing image...\n";
        t0 = clock.now();
        globalImage.write("spheres.png");
        t1 = clock.now();

        std::cout << "Done. (" << std::chrono::duration<double>(t1 - t0).count() << "s)\n";
    }

    MPI_Finalize();

    return 0;
}
