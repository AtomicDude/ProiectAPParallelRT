#pragma once

#include "VecSerialization.h"

using json = nlohmann::json;

namespace rt
{
    struct CameraSerialization
    {
    public:
        Vec3 position;
        Vec3 lookAt;
        Vec3 up;
        double fovy;
        double aperture;
    };

    void to_json(json& j, const CameraSerialization& c)
    {
        j = json
        {
            { "position", c.position },
            { "lookAt", c.lookAt },
            { "up", c.up },
            { "fovy", c.fovy },
            { "aperture", c.aperture }
        };
    }

    void from_json(const json& j, CameraSerialization& c)
    {
        j.at("position").get_to(c.position);
        j.at("lookAt").get_to(c.lookAt);
        j.at("up").get_to(c.up);
        j.at("fovy").get_to(c.fovy);
        j.at("aperture").get_to(c.aperture);
    }
}