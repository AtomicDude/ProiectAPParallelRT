#include "Camera.h"
#include "../Utility/Utility.h"

namespace rt
{
    Camera::Camera(
        const Vec3& eye,
        const Vec3& lookat,
        const Vec3& up,
        double fov,
        double ratio,
        double aperture
    ) :
        m_LensRadius(aperture / 2.0)
    {
        double theta = rad(fov);
        double height = 2.0 * tan(theta / 2.0);
        double width = ratio * height;

        m_Origin = eye;
        m_W = Vec3::normalize(eye - lookat);
        m_U = Vec3::normalize(Vec3::cross(up, m_W));
        m_V = Vec3::cross(m_W, m_U);

        m_Horizontal = width * m_U;
        m_Vertical = height * m_V;
        m_LowerLeft = m_Origin - m_Horizontal / 2.0 - m_Vertical / 2.0 - m_W;
    }

    Ray Camera::castRay(double s, double t) const
    {
        Vec3 rand = m_LensRadius * Vec3::randomInUnitDisk();
        Vec3 offsetOrigin = m_Origin + m_U * rand.x + m_V * rand.y;

        return Ray(offsetOrigin, m_LowerLeft + s * m_Horizontal + t * m_Vertical - offsetOrigin);
    }
}
