#include "OpenGL3DDataObject.h"
#include "Logger.h"
#include "Mesh3D.h"
#include "VertexAttribute.h"
#include "VertexColorAttribute.h"
#include "VertexNormalAttribute.h"

OpenGL3DDataObject::OpenGL3DDataObject(){
    CHECK_GL_ERROR_(glGenVertexArrays(1, &m_VAO))
        CHECK_GL_ERROR_(glGenBuffers(1, &m_VBO))}

OpenGL3DDataObject::~OpenGL3DDataObject(void)
{
    // Delete Array Buffers.
    CHECK_GL_ERROR_(glDeleteVertexArrays(1, &m_VAO))
    // Delete Indexbuffer
    CHECK_GL_ERROR_(glDeleteBuffers(1, &m_VBO))
}

void OpenGL3DDataObject::InitializeBuffer(Mesh3D& mesh)
{
    const std::vector<Eigen::Vector3f>& vertices      = mesh.GetVertices();
    const std::vector<uint32_t>&        vertexIndices = mesh.GetIndices();

    auto                                pNormal       = dynamic_cast<VertexNormalAttribute*>(mesh.GetVertexAttribute(Mesh3D::EVertexAttribute::Normal));
    const std::vector<Eigen::Vector3f>& normals       = pNormal->m_vertexNormals;
    const std::vector<uint32_t>&        normalIndices = pNormal->m_vertexNormalsIndices;

    auto                                pColor       = dynamic_cast<VertexColorAttribute*>(mesh.GetVertexAttribute(Mesh3D::EVertexAttribute::Color));
    const std::vector<Eigen::Vector3f>& color        = pColor->m_vertexColor;
    const std::vector<uint32_t>&        colorIndices = pColor->m_vertexColorIndices;

    ASSERT(vertexIndices.size() == normalIndices.size());
    ASSERT(normalIndices.size() == colorIndices.size());
    // position, normal,
    std::vector<float> bufferData;
    // each vertex, normal and color has 3 dimensions
    bufferData.resize(3 * (vertexIndices.size() + normalIndices.size() + colorIndices.size()));
    int j = 0;
    for (uint32_t i = 0; i < bufferData.size(); i += 9)
    {
        const Eigen::Vector3f& v = vertices[vertexIndices[j]];
        bufferData[i]            = v.x();
        bufferData[i + 1]        = v.y();
        bufferData[i + 2]        = v.z();
        ++j;
    }

    j = 0;
    for (uint32_t i = 3; i < bufferData.size(); i += 9)
    {
        const Eigen::Vector3f& n = normals[normalIndices[j]];
        bufferData[i]            = n.x();
        bufferData[i + 1]        = n.y();
        bufferData[i + 2]        = n.z();
        ++j;
    }

    j = 0;
    for (uint32_t i = 6; i < bufferData.size(); i += 9)
    {
        const Eigen::Vector3f& c = color[colorIndices[j]];
        bufferData[i]            = c.x();
        bufferData[i + 1]        = c.y();
        bufferData[i + 2]        = c.z();
        ++j;
    }

    m_vertexRenderCount = vertexIndices.size();

    CHECK_GL_ERROR_(glBindVertexArray(m_VAO))
    CHECK_GL_ERROR_(glBindBuffer(GL_ARRAY_BUFFER, m_VBO))
    CHECK_GL_ERROR_(glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(float), &bufferData[0], GL_STATIC_DRAW))

    CHECK_GL_ERROR_(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0))
    CHECK_GL_ERROR_(glEnableVertexAttribArray(0))

    CHECK_GL_ERROR_(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))))
    CHECK_GL_ERROR_(glEnableVertexAttribArray(1))

    CHECK_GL_ERROR_(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float))))
    CHECK_GL_ERROR_(glEnableVertexAttribArray(2))
    // unbind
    CHECK_GL_ERROR_(glBindVertexArray(0));
}

void OpenGL3DDataObject::DrawObject(GLenum mode) const
{
    CHECK_GL_ERROR_(glBindVertexArray(m_VAO))
    CHECK_GL_ERROR_(glDrawArrays(mode, 0, m_vertexRenderCount))
    CHECK_GL_ERROR_(glBindVertexArray(0))
}
