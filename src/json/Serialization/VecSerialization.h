#pragma once

#include "../../Vec/Vec3.h"
#include "../json.hpp"

using json = nlohmann::json;

namespace rt
{
    void to_json(json& j, const Vec3& v)
    {
        j = json
        {
            { "x", v.x },
            { "y", v.y },
            { "z", v.z }
        };
    }

    void from_json(const json& j, Vec3& v)
    {
        if (j.contains("x"))
        {
            j.at("x").get_to(v.x);
        }

        if (j.contains("y"))
        {
            j.at("y").get_to(v.y);
        }

        if (j.contains("z"))
        {
            j.at("z").get_to(v.z);
        }

        if (j.contains("r"))
        {
            j.at("r").get_to(v.r);
        }

        if (j.contains("g"))
        {
            j.at("g").get_to(v.g);
        }

        if (j.contains("b"))
        {
            j.at("b").get_to(v.b);
        }
    }
}