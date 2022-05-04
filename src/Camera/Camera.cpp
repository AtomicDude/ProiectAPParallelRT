#include "Camera.h"
#include "../Utility/Utility.h"

namespace rt
{
    Camera::Camera(const Vec3& eye, const Vec3& lookat, const Vec3& up, double fov, double ratio)
    {
        Vec3 v, u, w;

        double theta = rad(fov);
        double height = 2.0 * tan(theta / 2.0);
        double width = ratio * height;

        m_Origin = eye;
        w = Vec3::normalize(eye - lookat);
        u = Vec3::normalize(Vec3::cross(up, w));
        v = Vec3::cross(w, u);

        m_Horizontal = width * u;
        m_Vertical = height * v;
        m_LowerLeft = m_Origin - m_Horizontal / 2.0 - m_Vertical / 2.0 - w;
    }

    Ray Camera::castRay(double u, double v) const
    {
        return Ray(m_Origin, m_LowerLeft + u * m_Horizontal + v * m_Vertical - m_Origin);
    }
}
