#ifndef QUADER_H
#define QUADER_H

#include "Primitives3D.h"
#include <Eigen/Dense>
#include <memory>
#include <vector>

class Quader : public Primitives3D
{
public:
    explicit Quader(float width, float height, float depth);
    std::shared_ptr<Mesh3D> GetMesh() override;

private:
    std::vector<Eigen::Vector3f> m_vertices;
    std::vector<uint32_t>        m_indices;
    float                        m_width  = 1.f;
    float                        m_height = 1.f;
    float                        m_depth  = 1.f;
};

#endif
