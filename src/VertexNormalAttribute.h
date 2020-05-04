#include "VertexAttribute.h"
#include "Macros.h"
#include <algorithm>

class VertexNormalAttribute : public VertexAttribute
{
public:
    VertexNormalAttribute(const std::vector<Eigen::Vector3f>& vertices, const std::vector<uint32_t>& indices)
        : m_indices{indices}
        , m_vertices{vertices}
    {
        m_vertexNormals.resize(m_vertices.size());
    }

    Eigen::Vector3f& operator[](uint32_t i)
    {
        return m_vertexNormals[i];
    }

    void SetVertexNormals(const std::vector<Eigen::Vector3f> &vertexNormals)
	{
		ASSERT(m_vertexNormals.size() == m_vertexNormals.size());
		std::copy(std::begin(vertexNormals), std::end(vertexNormals),std::begin(m_vertexNormals));
	}

private:
	friend class OpenGL3DDataObject;
    std::vector<Eigen::Vector3f>        m_vertexNormals;
    const std::vector<uint32_t>&        m_indices;
    const std::vector<Eigen::Vector3f>& m_vertices;
};
