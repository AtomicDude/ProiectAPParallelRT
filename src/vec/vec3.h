#pragma once

#include <fstream>
#include <math.h>

namespace rt
{
    union vec3
    {
        vec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) :
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

    inline vec3 operator+(const vec3& l, const vec3& r)
    {
        return vec3(
            l.x + r.x,
            l.y + r.y,
            l.z + r.z
            );
    }

    inline vec3 operator-(const vec3& l, const vec3& r)
    {
        return vec3(
            l.x - r.x,
            l.y - r.y,
            l.z - r.z
            );
    }

    inline vec3 operator*(float s, vec3 v)
    {
        return vec3(
            s * v.x,
            s * v.y,
            s * v.z
            );
    }

    inline vec3 operator*(vec3 v, float s)
    {
        return vec3(
            v.x * s,
            v.y * s,
            v.z * s
            );
    }

    inline vec3 operator/(vec3 v, float s)
    {
        return vec3(
            v.x / s,
            v.y / s,
            v.z / s
            );
    }

    inline vec3& operator+=(vec3& l, const vec3& r)
    {
        l.x += r.x;
        l.y += r.y;
        l.z += r.z;

        return l;
    }

    inline vec3& operator-=(vec3& l, const vec3& r)
    {
        l.x -= r.x;
        l.y -= r.y;
        l.z -= r.z;

        return l;
    }

    inline vec3& operator*=(vec3& l, float s)
    {
        l.x *= s;
        l.y *= s;
        l.z *= s;

        return l;
    }

    inline vec3& operator/=(vec3& l, float s)
    {
        l.x /= s;
        l.y /= s;
        l.z /= s;

        return l;
    }

    inline float len(const vec3& v)
    {
        return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    inline float len2(const vec3& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }
    
    inline vec3 normalize(const vec3& v)
    {
        return v / len(v);
    }

    inline float dot(const vec3& l, const vec3& r)
    {
        return l.x * r.x + l.y * r.y + l.z * r.z;
    }

    inline vec3 cross(const vec3& l, const vec3& r)
    {
        return vec3(
            l.y * r.z - l.z * r.y,
            l.x * r.z + l.z * r.x,
            l.x * r.y - l.y * r.x
        );
    }

    inline vec3 lerp(float t, const vec3& l, const vec3& r)
    {
        return (1.0f - t) * l + t * r;
    }

    inline std::ostream& operator<<(std::ostream& out, const vec3 v)
    {
        out << "(" << v.x << ", " << v.y << ", " << v.z << ")";

        return out;
    }
}