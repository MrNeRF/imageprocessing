#ifndef AABB_H
#define AABB_H

#include "Ray.h"
#include <Eigen/Dense>
#include <limits>
#include <optional>

class AABB
{
public:
    struct Intersection
    {
        Intersection(float tmin, const Eigen::Vector3f hitPoint)
            : m_tmin{tmin}
            , m_hitPoint{hitPoint}
        {
        }
        float           m_tmin;
        Eigen::Vector3f m_hitPoint;
    };

public:
    AABB(const Eigen::Vector3f& min, const Eigen::Vector3f& max)
        : m_min{min}
        , m_max{max}
    {
    }

    bool                        TestABBOverlapping(const AABB& aabb);
    std::optional<Intersection> IntersectRayAABB(const Ray& rRay, const Eigen::Vector3f& rTranslate);

private:
    Eigen::Vector3f m_min;
    Eigen::Vector3f m_max;
};

#endif
