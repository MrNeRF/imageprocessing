#ifndef VERTEX_COLOR_ATTRIBUTE
#define VERTEX_COLOR_ATTRIBUTE

#include "Eigen/Dense"
#include "VertexAttribute.h"
#include <vector>

class VertexColorAttribute : public VertexAttribute
{
public:
    VertexColorAttribute(const std::vector<Eigen::Vector3f>& vertices, const std::vector<uint32_t>& indices)
        : m_indices{indices}
        , m_vertices{vertices}
    {
        m_vertexColor.resize(m_vertices.size());
    }
    Eigen::Vector3f& operator[](uint32_t i)
    {
        return m_vertexColor[i];
    }

private:
    std::vector<Eigen::Vector3f>        m_vertexColor;
    const std::vector<uint32_t>&        m_indices;
    const std::vector<Eigen::Vector3f>& m_vertices;
};

#endif
