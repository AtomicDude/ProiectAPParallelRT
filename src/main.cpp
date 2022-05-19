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
#include <mpi/mpi.h>

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
    uint32_t width_granularity = sceneParams.params.granularity;
    bool useBorder = sceneParams.params.border;
    const uint32_t width = static_cast<uint32_t>(ratio * static_cast<double>(height));
    const uint32_t channels = 3;
    uint32_t height_granularity = width_granularity;

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

    std::vector<rt::Area> renderAreas;

    MPI_Init(&argc, &argv);

    int32_t comm_size;
    int32_t comm_rank;
    const int32_t root = 0;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_size(comm, &comm_size);
    MPI_Comm_rank(comm, &comm_rank);

    int32_t default_area_height = static_cast<int32_t>(height / (comm_size * width_granularity));
    default_area_height = default_area_height == 0 ? 1 : default_area_height;

    int32_t default_area_width = static_cast<int32_t>(width / (comm_size * height_granularity));
    default_area_width = default_area_width == 0 ? 1 : default_area_width;

    useBorder = (default_area_height == 1 || default_area_width == 1) ? false : useBorder;

    for (uint32_t y = 0; y < height; y += default_area_height)
    {
        for (uint32_t x = 0; x < width; x += default_area_width)
        {
            renderAreas.push_back(
                rt::Area(
                    x,
                    y,
                    (x + default_area_width) < width ? default_area_width : (width - x),
                    (y + default_area_height) < height ? default_area_height : (height - y)));
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
            rt::Area &area = renderAreas[i];

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

    std::vector<rt::Image> localImages;
    std::vector<MPI_Request> sendRequests;
    localImages.reserve(renderAreas.size());
    sendRequests.reserve(renderAreas.size());

    for (uint32_t i = comm_rank; i < renderAreas.size(); i += comm_size)
    {
        std::cout << "Start " << comm_rank << std::endl;
        rt::Area &area = renderAreas[i];

        int32_t index = MPI_UNDEFINED;
        int32_t flag = 0;
        MPI_Testany(sendRequests.size(), sendRequests.data(), &index, &flag, MPI_STATUS_IGNORE);

        if (index == MPI_UNDEFINED)
        {
            MPI_Request request;
            sendRequests.push_back(request);
            localImages.emplace_back(channels);

            index = sendRequests.size() - 1;
        }

        localImages[index].resize(area.width, area.height);

        scene.render(
            localImages[index],                      // outImage
            camera,                                  // camera
            area.x,                                  // x_start
            area.y,                                  // y_start
            rt::Area(0, 0, area.width, area.height), // imageArea
            sceneParams.params.samples,              // samples per pixel
            sceneParams.params.bounces,              // max ray depth (bounces)
            sceneParams.params.gamma,                // gamma correction
            useBorder                                // border
        );

        std::cout << "Stop " << comm_rank << std::endl;

        MPI_Isend(localImages[index].data(), localImages[index].size(), MPI_BYTE, root, i, comm, &sendRequests[index]);
    }

    if (comm_rank == root)
    {
        MPI_Waitall(requests.size(), requests.data(), MPI_STATUSES_IGNORE);

        auto t1 = clock.now();
        std::cout << "Done. (" << std::chrono::duration<double>(t1 - t0).count() << "s)\n";

        std::cout << "Writing image...\n";
        t0 = clock.now();
        globalImage.write(sceneParams.title + ".png");
        t1 = clock.now();

        std::cout << "Done. (" << std::chrono::duration<double>(t1 - t0).count() << "s)\n";
    }

    MPI_Finalize();

    return 0;
}
