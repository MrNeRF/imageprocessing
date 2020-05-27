#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>

struct Ray
{
    Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f direction, float t)
        : m_origin{origin}
        , m_direction{direction}
        , m_t{t}
    {
    }
    Eigen::Vector3f m_origin;
    Eigen::Vector3f m_direction;
    float           m_t = 0.f;
};

#endif
