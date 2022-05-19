#pragma once

#include "VecSerialization.h"
#include "MaterialSerialization.h"

using json = nlohmann::json;

namespace rt
{
    struct DrawableParams
    {
    public:
        Vec3 position;
        double width;
        double height;
        double depth;

        Vec3 center;
        double radius;

        MaterialSerialization material;
    };

    struct DrawableSerialization
    {
    public:
        std::string type;
        DrawableParams params;
    };

    void to_json(json& j, const DrawableParams& p)
    {
        j = json
        {
            { "position", p.position },
            { "width", p.width },
            { "height", p.height },
            { "depth", p.depth },
            { "center", p.center },
            { "radius", p.radius },
            { "material", p.material }
        };
    }

    void from_json(const json& j, DrawableParams& p)
    {
        if (j.contains("position"))
        {
            j.at("position").get_to(p.position);
        }

        if (j.contains("width"))
        {
            j.at("width").get_to(p.width);
        }

        if (j.contains("height"))
        {
            j.at("height").get_to(p.height);
        }

        if (j.contains("depth"))
        {
            j.at("depth").get_to(p.depth);
        }

        if (j.contains("center"))
        {
            j.at("center").get_to(p.center);
        }

        if (j.contains("radius"))
        {
            j.at("radius").get_to(p.radius);
        }

        if (j.contains("material"))
        {
            j.at("material").get_to(p.material);
        }
    }

    void to_json(json& j, const DrawableSerialization& d)
    {
        j = json
        {
            { "type", d.type },
            { "params", d.params }
        };
    }

    void from_json(const json& j, DrawableSerialization& d)
    {
        j.at("type").get_to(d.type);
        j.at("params").get_to(d.params);
    }
}