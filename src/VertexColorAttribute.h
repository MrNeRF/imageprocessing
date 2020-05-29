#ifndef VERTEX_COLOR_ATTRIBUTE
#define VERTEX_COLOR_ATTRIBUTE

#include "Eigen/Dense"
#include "VertexAttribute.h"
#include <vector>

class VertexColorAttribute : public VertexAttribute
{
public:
    Eigen::Vector3f& operator[](uint32_t i)
    {
        return m_vertexColor[m_vertexColorIndices[i]];
    }

    void SetVertexColor(const std::vector<Eigen::Vector3f>& vertexColor,
                        const std::vector<uint32_t>         vertexColorIndices)
    {
        m_vertexColor.resize(vertexColor.size());
        m_vertexColorIndices.resize(vertexColorIndices.size());
        std::copy(std::begin(vertexColor), std::end(vertexColor), std::begin(m_vertexColor));
        std::copy(std::begin(vertexColorIndices), std::end(vertexColorIndices), std::begin(m_vertexColorIndices));
    }

private:
    friend class OpenGL3DDataObject;
    std::vector<Eigen::Vector3f> m_vertexColor;
    std::vector<uint32_t>        m_vertexColorIndices;
};

#endif
