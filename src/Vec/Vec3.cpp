#include "Vec3.h"
#include "../Utility/Utility.h"

namespace rt
{
    double Vec3::len(const Vec3& v)
    {
        return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    double Vec3::len2(const Vec3& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }

    Vec3 Vec3::normalize(const Vec3& v)
    {
        return v / len(v);
    }

    double Vec3::dot(const Vec3& l, const Vec3& r)
    {
        return l.x * r.x + l.y * r.y + l.z * r.z;
    }

    Vec3 Vec3::cross(const Vec3& l, const Vec3& r)
    {
        return Vec3(
            l.y * r.z - l.z * r.y,
            l.x * r.z + l.z * r.x,
            l.x * r.y - l.y * r.x
        );
    }

    Vec3 Vec3::reflect(const Vec3& v, const Vec3& normal)
    {
        return v - 2.0 * dot(v, normal) * normal;
    }

    Vec3 Vec3::refract(const Vec3& v, const Vec3& normal, double ratio)
    {
        double cos_theta = fmin(dot(-v, normal), 1.0);

        Vec3 v_perpendicular = ratio * (v + cos_theta * normal);
        Vec3 v_parallel = -sqrt(fabs(1.0 - Vec3::len2(v_perpendicular))) * normal;

        return v_perpendicular + v_parallel;
    }

    Vec3 Vec3::lerp(double t, const Vec3& l, const Vec3& r)
    {
        return (1.0 - t) * l + t * r;
    }

    Vec3 Vec3::randomUnit()
    {
        return normalize(Vec3(random_double(), random_double(), random_double()));
    }

    Vec3 Vec3::randomInUnitSphere()
    {
        Vec3 v;

        do
        {
            v = Vec3(
                random_double(-1.0, 1.0),
                random_double(-1.0, 1.0),
                random_double(-1.0, 1.0)
            );
        } while (len2(v) > 1.0);

        return v;
    }

    Vec3 Vec3::randomInUnitDisk()
    {
        Vec3 v;

        do
        {
            v = Vec3(
                random_double(-1.0, 1.0),
                random_double(-1.0, 1.0),
                0.0
            );
        } while (len2(v) > 1.0);

        return v;
    }

    bool Vec3::isNearZero(const Vec3& v, double eps)
    {
        return fabs(v.x) < eps && fabs(v.y) < eps && fabs(v.z) < eps;
    }

    // vector operators
    Vec3 operator+(const Vec3& l, const Vec3& r)
    {
        return Vec3(
            l.x + r.x,
            l.y + r.y,
            l.z + r.z
        );
    }

    Vec3 operator-(const Vec3& l, const Vec3& r)
    {
        return Vec3(
            l.x - r.x,
            l.y - r.y,
            l.z - r.z
        );
    }

    Vec3 operator-(const Vec3& v)
    {
        return Vec3(
            -v.x,
            -v.y,
            -v.z
        );
    }

    Vec3& operator+=(Vec3& l, const Vec3& r)
    {
        l.x += r.x;
        l.y += r.y;
        l.z += r.z;

        return l;
    }

    Vec3& operator-=(Vec3& l, const Vec3& r)
    {
        l.x -= r.x;
        l.y -= r.y;
        l.z -= r.z;

        return l;
    }

    // scalar operators
    Vec3 operator*(double s, Vec3 v)
    {
        return Vec3(
            s * v.x,
            s * v.y,
            s * v.z
        );
    }

    Vec3 operator*(Vec3 v, double s)
    {
        return Vec3(
            v.x * s,
            v.y * s,
            v.z * s
        );
    }

    Vec3 operator/(Vec3 v, double s)
    {
        return Vec3(
            v.x / s,
            v.y / s,
            v.z / s
        );
    }

    Vec3& operator*=(Vec3& l, double s)
    {
        l.x *= s;
        l.y *= s;
        l.z *= s;

        return l;
    }

    Vec3& operator/=(Vec3& l, double s)
    {
        l.x /= s;
        l.y /= s;
        l.z /= s;

        return l;
    }

    // component-wise operators
    Vec3 operator*(const Vec3& l, const Vec3& r)
    {
        return Vec3(
            l.x * r.x,
            l.y * r.y,
            l.z * r.z
        );
    }

    Vec3 operator/(const Vec3& l, const Vec3& r)
    {
        return Vec3(
            l.x / r.x,
            l.y / r.y,
            l.z / r.z
        );
    }

    Vec3& operator*=(Vec3& l, const Vec3& r)
    {
        l.x *= r.x;
        l.y *= r.y;
        l.z *= r.z;

        return l;
    }

    Vec3& operator/=(Vec3& l, const Vec3& r)
    {
        l.x /= r.x;
        l.y /= r.y;
        l.z /= r.z;

        return l;
    }

    // other
    std::ostream& operator<<(std::ostream& out, const Vec3 v)
    {
        out << "(" << v.x << ", " << v.y << ", " << v.z << ")";

        return out;
    }
}
