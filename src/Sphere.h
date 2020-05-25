#ifndef SPHERE_H
#define SPHERE_H

#include "Primitives3D.h"
#include <Eigen/Dense>
#include <memory>
#include <vector>

class Sphere : public Primitives3D
{
public:
    explicit Sphere(float radius, uint32_t stackCount, uint32_t sectorCount);
    std::shared_ptr<Mesh3D> GetMesh() override;

private:
    std::vector<Eigen::Vector3f> m_vertices;
	std::vector<Eigen::Vector3f> m_normals;
    std::vector<uint32_t>        m_indices;
    uint32_t                     m_radius  = 1.f;
    uint32_t					 m_stackCount = 10;
    uint32_t                     m_sectorCount  = 15;
};

#endif
