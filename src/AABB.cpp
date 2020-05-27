#include "AABB.h"
#include <iostream>

bool AABB::TestABBOverlapping(const AABB& aabb)
{
    if (m_max[0] < aabb.m_min[0] || m_min[0] > aabb.m_max[0])
        return false;
    if (m_max[1] < aabb.m_min[1] || m_min[1] > aabb.m_max[1])
        return false;
    if (m_max[2] < aabb.m_min[2] || m_min[2] > aabb.m_max[2])
        return false;
    return true;
}

std::optional<AABB::Intersection> AABB::IntersectRayAABB(const Ray& rRay, const Eigen::Vector3f& rTranslate)
{
    float tmin = 0.f;
    float tmax = std::numeric_limits<float>::max();

    Eigen::Vector3f min = m_min + rTranslate;
    Eigen::Vector3f max = m_max + rTranslate;

    for (uint32_t i = 0; i < 3; ++i)
    {
        if (std::abs(rRay.m_direction[i]) < std::numeric_limits<float>::epsilon())
        {
            if (rRay.m_origin[i] < min[i] || rRay.m_origin[i] > max[i])
                return std::nullopt;
        }
        else
        {
            float r  = 1.f / rRay.m_direction[i];
            float t1 = (min[i] - rRay.m_origin[i]) * r;
            float t2 = (max[i] - rRay.m_origin[i]) * r;
            if (t1 > t2)
            {
                std::swap(t1, t2);
            }

            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);
            if (tmin > tmax)
            {
                return std::nullopt;
            }
        }
    }
    return Intersection(tmin, rRay.m_origin + tmin * rRay.m_direction);
}
