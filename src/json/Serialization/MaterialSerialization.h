#pragma once

#include "VecSerialization.h"

using json = nlohmann::json;

namespace rt
{
    struct MaterialParams
    {
    public:
        Vec3 color;
        double fuzziness;
        double refraction;
    };

    struct MaterialSerialization
    {
    public:
        std::string type;
        MaterialParams params;
    };

    void to_json(json& j, const MaterialParams& p)
    {
        j = json
        {
            { "color", p.color },
            { "fuzziness", p.fuzziness },
            { "refraction", p.refraction }
        };
    }

    void from_json(const json& j, MaterialParams& p)
    {
        j.at("color").get_to(p.color);

        if (j.contains("fuzziness"))
        {
            j.at("fuzziness").get_to(p.fuzziness);
        }

        if (j.contains("refraction"))
        {
            j.at("refraction").get_to(p.refraction);
        }
    }

    void to_json(json& j, const MaterialSerialization& m)
    {
        j = json
        {
            { "type", m.type },
            { "params", m.params }
        };
    }

    void from_json(const json& j, MaterialSerialization& m)
    {
        j.at("type").get_to(m.type);
        j.at("params").get_to(m.params);
    }
}