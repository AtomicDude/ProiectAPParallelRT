#pragma once

#include "../Drawable/DrawableList/DrawableList.h"
#include "../Camera/Camera.h"

#include <string>
#include <vector>

namespace rt
{
    class Image
    {
    public:
        Image(int _width, int _height);
        void setBackgroundGradient(const Vec3& _upColor, const Vec3& _downColor);
        void emplaceSphere(const Vec3& c, float r);
        void render(const std::string& path, const Camera& camera, int antialiasing = 16);

    private:
        Vec3 pixel(const Ray& ray) const;

    private:
        int width;
        int height;
        DrawableList drawableList;
        Vec3 upColor;
        Vec3 downColor;
    };
}