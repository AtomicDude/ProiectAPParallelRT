#pragma once

#include "vec3.h"
#include "ray.h"
#include <string>
#include <vector>

namespace rt
{
    vec3 gradient_color(const ray& ray, const vec3& color1, const vec3& color2);
    void write_image(const std::string file_name, const std::vector<std::vector<vec3>>& pixels);
}