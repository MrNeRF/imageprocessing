#include "OpenGL3DDataObject.h"
#include "Mesh3D.h"
#include "VertexAttribute.h"
#include "VertexNormalAttribute.h"
#include "Logger.h"

OpenGL3DDataObject::OpenGL3DDataObject() 
{ 
	CHECK_GL_ERROR_(glGenVertexArrays(1, &VAO)) 
}

OpenGL3DDataObject::~OpenGL3DDataObject(void)
{
    // Delete Array Buffers.
    CHECK_GL_ERROR_(glDeleteBuffers(buffersInUseVector.size(), &buffersInUseVector[0]))
    // Delete Indexbuffer
    CHECK_GL_ERROR_(glDeleteBuffers(1, &indexBuffer))
}

void OpenGL3DDataObject::InitializeVertexBuffer(Mesh3D& mesh)
{
    const auto& indices  = mesh.m_indices;
    const auto& vertices = mesh.m_vertices;
    numberOfIndices  = indices.size();
    numberOfVertices = vertices.size();

    CHECK_GL_ERROR_(glGenVertexArrays(1, &VAO))
    CHECK_GL_ERROR_(glGenBuffers(1, &vertexBuffer));
    CHECK_GL_ERROR_(glGenBuffers(1, &indexBuffer));

    buffersInUseVector.push_back(vertexBuffer);

    CHECK_GL_ERROR_(glBindVertexArray(VAO));

    constexpr unsigned int dimension = 3;
    CHECK_GL_ERROR_(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    CHECK_GL_ERROR_(glBufferData(GL_ARRAY_BUFFER, dimension * sizeof(float) * numberOfVertices, vertices[0].data(), GL_STATIC_DRAW));

    // In Opengl there is only one index Buffer per VAO
    CHECK_GL_ERROR_(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    CHECK_GL_ERROR_(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numberOfIndices, &indices[0], GL_STATIC_DRAW));

    // Dimension of Vertex Data
    CHECK_GL_ERROR_(glVertexAttribPointer(vertexAttrIdx, dimension, GL_FLOAT, GL_FALSE, 0, (void*)0));
    CHECK_GL_ERROR_(glEnableVertexAttribArray(vertexAttrIdx));

    // unbind
    CHECK_GL_ERROR_(glBindVertexArray(0));
}

void OpenGL3DDataObject::InitializeColorBuffer(const Color& color)
{
    if (colorBuffer == 0)
    {
        CHECK_GL_ERROR_(glGenBuffers(1, &colorBuffer))
        buffersInUseVector.push_back(colorBuffer);
    }

    // Dimension of Color Data
    constexpr unsigned int dimension = 3;

    Eigen::Matrix<float, Eigen::Dynamic, dimension, Eigen::RowMajor> colorData;
    colorData = color.GetColor().replicate(1, numberOfVertices).transpose();
    CHECK_GL_ERROR_(glBindVertexArray(VAO))

    CHECK_GL_ERROR_(glBindBuffer(GL_ARRAY_BUFFER, colorBuffer))
    CHECK_GL_ERROR_(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorData.size(), colorData.data(), GL_STATIC_DRAW))

    CHECK_GL_ERROR_(glVertexAttribPointer(colorAttrIdx, dimension, GL_FLOAT, GL_FALSE, 0, (void*)0))
    CHECK_GL_ERROR_(glEnableVertexAttribArray(colorAttrIdx))
    CHECK_GL_ERROR_(glBindVertexArray(0))
}

void OpenGL3DDataObject::InitializeTextureUVBuffer(const Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>& uvCoordinates)
{
    if (textureBuffer == 0)
    {
        CHECK_GL_ERROR_(glGenBuffers(1, &textureBuffer));
        buffersInUseVector.push_back(textureBuffer);
    }

    // Dimension of Color Data
    constexpr unsigned int dimension = 2;

    CHECK_GL_ERROR_(glBindVertexArray(VAO))

    CHECK_GL_ERROR_(glBindBuffer(GL_ARRAY_BUFFER, textureBuffer))
    CHECK_GL_ERROR_(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvCoordinates.size(), uvCoordinates.data(), GL_STATIC_DRAW))

    CHECK_GL_ERROR_(glVertexAttribPointer(textureAttrIdx, dimension, GL_FLOAT, GL_FALSE, 0, (void*)0))
    CHECK_GL_ERROR_(glEnableVertexAttribArray(textureAttrIdx))
    CHECK_GL_ERROR_(glBindVertexArray(0))
}

void OpenGL3DDataObject::InitializeNormalBuffer(Mesh3D& mesh)
{
    if (!mesh.HasNormals())
    {
        return;
    }
    if (normalsBuffer == 0)
    {
        CHECK_GL_ERROR_(glGenBuffers(1, &normalsBuffer))
        buffersInUseVector.push_back(normalsBuffer);
    }

    VertexNormalAttribute* pVertexNormalAttribute = dynamic_cast<VertexNormalAttribute*>(mesh.GetVertexAttribute(Mesh3D::EVertexAttribute::Normal));
    if (pVertexNormalAttribute == nullptr)
    {
        return;
    }
    // Dimension of Normals Data
    constexpr unsigned int dimension = 3;

    CHECK_GL_ERROR_(glBindVertexArray(VAO))

    CHECK_GL_ERROR_(glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer))
    CHECK_GL_ERROR_(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dimension * pVertexNormalAttribute->m_vertexNormals.size(), pVertexNormalAttribute->m_vertexNormals[0].data(), GL_STATIC_DRAW))

    CHECK_GL_ERROR_(glVertexAttribPointer(normalAttrIdx, dimension, GL_FLOAT, GL_FALSE, 0, (void*)0))
    CHECK_GL_ERROR_(glEnableVertexAttribArray(normalAttrIdx))
    CHECK_GL_ERROR_(glBindVertexArray(0))
}

void OpenGL3DDataObject::DrawObject(GLenum mode) const
{
    if (numberOfIndices == 0)
    {
        assert(false && "There are no vertices");
    }

    CHECK_GL_ERROR_(glBindVertexArray(VAO))
    CHECK_GL_ERROR_(glDrawElements(mode, numberOfIndices, GL_UNSIGNED_INT, NULL))
    CHECK_GL_ERROR_(glBindVertexArray(0))
}
