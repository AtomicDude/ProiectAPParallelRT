#pragma once

#include "DrawableSerialization.h"
#include "CameraSerialization.h"

#include <vector>

using json = nlohmann::json;

namespace rt
{
    struct RenderParams
    {
    public:
        uint32_t height;
        int32_t granularity;
        bool border;
        uint32_t samples;
        uint32_t bounces;
        double gamma;
        CameraSerialization camera;
    };

    struct SceneSerialization
    {
    public:
        std::string title;
        RenderParams params;
        std::vector<DrawableSerialization> drawables;
    };

    void to_json(json& j, const RenderParams& p)
    {
        j = json
        {
            { "height", p.height },
            { "granularity", p.granularity },
            { "border", p.border },
            { "samples", p.samples },
            { "bounces", p.bounces },
            { "gamma", p.gamma },
            { "camera", p.camera }
        };
    }

    void from_json(const json& j, RenderParams& p)
    {
        j.at("height").get_to(p.height);
        j.at("granularity").get_to(p.granularity);
        j.at("border").get_to(p.border);
        j.at("samples").get_to(p.samples);
        j.at("bounces").get_to(p.bounces);
        j.at("gamma").get_to(p.gamma);
        j.at("camera").get_to(p.camera);
    }

    void to_json(json& j, const SceneSerialization& s)
    {
        j = json
        {
            { "title", s.title },
            { "params", s.params },
            { "drawables", s.drawables }
        };
    }

    void from_json(const json& j, SceneSerialization& s)
    {
        j.at("title").get_to(s.title);
        j.at("params").get_to(s.params);
        j.at("drawables").get_to(s.drawables);
    }
}