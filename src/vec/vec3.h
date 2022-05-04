#pragma once

#include <ostream>

namespace rt
{
    union Vec3
    {
        Vec3(double _x = 0.0, double _y = 0.0, double _z = 0.0) :
            x(_x),
            y(_y),
            z(_z)
        {
        }

        struct
        {
            double x;
            double y;
            double z;
        };

        struct
        {
            double r;
            double g;
            double b;
        };

        double e[3];

        static double len(const Vec3& v);
        static double len2(const Vec3& v);
        static Vec3 normalize(const Vec3& v);
        static double dot(const Vec3& l, const Vec3& r);
        static Vec3 cross(const Vec3& l, const Vec3& r);
        static Vec3 reflect(const Vec3& v, const Vec3& normal);
        static Vec3 refract(const Vec3& v, const Vec3& normal, double ratio);
        static Vec3 lerp(double t, const Vec3& l, const Vec3& r);
        static Vec3 randomUnit();
        static Vec3 randomInUnitSphere();
        static Vec3 randomInUnitDisk();
        static bool isNearZero(const Vec3& v, double eps = 1e-7);
    };

    // vector operators
    Vec3 operator+(const Vec3& l, const Vec3& r);
    Vec3 operator-(const Vec3& l, const Vec3& r);
    Vec3 operator-(const Vec3& v);
    Vec3& operator+=(Vec3& l, const Vec3& r);
    Vec3& operator-=(Vec3& l, const Vec3& r);

    // scalar operators
    Vec3 operator*(double s, Vec3 v);
    Vec3 operator*(Vec3 v, double s);
    Vec3 operator/(Vec3 v, double s);
    Vec3& operator*=(Vec3& l, double s);
    Vec3& operator/=(Vec3& l, double s);

    // component-wise operators
    Vec3 operator*(const Vec3& l, const Vec3& r);
    Vec3 operator/(const Vec3& l, const Vec3& r);
    Vec3& operator*=(Vec3& l, const Vec3& r);
    Vec3& operator/=(Vec3& l, const Vec3& r);

    // other
    std::ostream& operator<<(std::ostream& out, const Vec3 v);
}