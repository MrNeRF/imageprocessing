#include "Logger.h"
#include "Macros.h"
#include "VertexAttribute.h"

class VertexNormalAttribute : public VertexAttribute
{
public:
    Eigen::Vector3f& operator[](uint32_t i)
    {
        return m_vertexNormals[m_vertexNormalsIndices[i]];
    }

    void SetVertexNormals(const std::vector<Eigen::Vector3f>& vertexNormals,
                          const std::vector<uint32_t>         vertexNormalsIndics)
    {
        m_vertexNormals.resize(vertexNormals.size());
        m_vertexNormalsIndices.resize(vertexNormalsIndics.size());
        std::copy(std::begin(vertexNormals), std::end(vertexNormals), std::begin(m_vertexNormals));
        std::copy(std::begin(vertexNormalsIndics), std::end(vertexNormalsIndics), std::begin(m_vertexNormalsIndices));
    }

private:
    friend class OpenGL3DDataObject;
    std::vector<Eigen::Vector3f> m_vertexNormals;
    std::vector<uint32_t>        m_vertexNormalsIndices;
};
