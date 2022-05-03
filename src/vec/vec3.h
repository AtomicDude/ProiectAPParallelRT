#pragma once

#include <fstream>
#include <math.h>

namespace rt
{
    union Vec3
    {
        Vec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) :
            x(_x),
            y(_y),
            z(_z)
        {
        }

        struct
        {
            float x;
            float y;
            float z;
        };

        struct
        {
            float r;
            float g;
            float b;
        };

        float e[3];
    };

    inline Vec3 operator+(const Vec3& l, const Vec3& r)
    {
        return Vec3(
            l.x + r.x,
            l.y + r.y,
            l.z + r.z
            );
    }

    inline Vec3 operator-(const Vec3& l, const Vec3& r)
    {
        return Vec3(
            l.x - r.x,
            l.y - r.y,
            l.z - r.z
            );
    }

    inline Vec3 operator*(float s, Vec3 v)
    {
        return Vec3(
            s * v.x,
            s * v.y,
            s * v.z
            );
    }

    inline Vec3 operator*(Vec3 v, float s)
    {
        return Vec3(
            v.x * s,
            v.y * s,
            v.z * s
            );
    }

    inline Vec3 operator/(Vec3 v, float s)
    {
        return Vec3(
            v.x / s,
            v.y / s,
            v.z / s
            );
    }

    inline Vec3& operator+=(Vec3& l, const Vec3& r)
    {
        l.x += r.x;
        l.y += r.y;
        l.z += r.z;

        return l;
    }

    inline Vec3& operator-=(Vec3& l, const Vec3& r)
    {
        l.x -= r.x;
        l.y -= r.y;
        l.z -= r.z;

        return l;
    }

    inline Vec3& operator*=(Vec3& l, float s)
    {
        l.x *= s;
        l.y *= s;
        l.z *= s;

        return l;
    }

    inline Vec3& operator/=(Vec3& l, float s)
    {
        l.x /= s;
        l.y /= s;
        l.z /= s;

        return l;
    }

    inline float len(const Vec3& v)
    {
        return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    inline float len2(const Vec3& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }
    
    inline Vec3 normalize(const Vec3& v)
    {
        return v / len(v);
    }

    inline float dot(const Vec3& l, const Vec3& r)
    {
        return l.x * r.x + l.y * r.y + l.z * r.z;
    }

    inline Vec3 cross(const Vec3& l, const Vec3& r)
    {
        return Vec3(
            l.y * r.z - l.z * r.y,
            l.x * r.z + l.z * r.x,
            l.x * r.y - l.y * r.x
        );
    }

    inline Vec3 lerp(float t, const Vec3& l, const Vec3& r)
    {
        return (1.0f - t) * l + t * r;
    }

    inline std::ostream& operator<<(std::ostream& out, const Vec3 v)
    {
        out << "(" << v.x << ", " << v.y << ", " << v.z << ")";

        return out;
    }
}