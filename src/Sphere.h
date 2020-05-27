#ifndef SPHERE_H
#define SPHERE_H

#include "Primitives3D.h"
#include "Ray.h"
#include <Eigen/Dense>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

class Sphere : public Primitives3D
{
public:
    explicit Sphere() = default;
    explicit Sphere(float radius)
        : m_radius{radius}
    {
    }
    explicit Sphere(const Eigen::Vector3f& p0);
    explicit Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1);
    explicit Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2);
    explicit Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& p3);
    const float                      GetRadius() { return m_radius; }
    const Eigen::Vector3f&           GetCenter() { return m_center; }
    void                             SetSphereResolution(uint32_t stackCount, uint32_t sectorCount);
    std::shared_ptr<Mesh3D>          CreateMesh() override;
    std::pair<bool, Eigen::Vector3f> IsHit(const Ray& rRay, Eigen::Vector3f position);

private:
    std::vector<Eigen::Vector3f> m_vertices;
    std::vector<Eigen::Vector3f> m_normals;
    Eigen::Vector3f              m_center;
    float                        m_radius = std::numeric_limits<float>::epsilon();
    std::vector<uint32_t>        m_indices;
    uint32_t                     m_stackCount  = 10;
    uint32_t                     m_sectorCount = 15;
};

#endif
