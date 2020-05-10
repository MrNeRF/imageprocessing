#include "OpenGL3DDataObject.h"
#include "Mesh3D.h"
#include "VertexAttribute.h"
#include "VertexNormalAttribute.h"
#include <cassert>

static void checkBufferCreationError(unsigned int bufferObjectID)
{
    if (bufferObjectID == 0)
    {
        assert(false && "Creating Buffer Object failed!\n");
    }
}

OpenGL3DDataObject::~OpenGL3DDataObject(void)
{
    // Delete Array Buffers.
    glDeleteBuffers(buffersInUseVector.size(), &buffersInUseVector[0]);
    // Delete Indexbuffer
    glDeleteBuffers(1, &indexBuffer);
}

void OpenGL3DDataObject::InitializeVertexBuffer(Mesh3D& mesh)
{
    const auto& indices  = mesh.m_indices;
    const auto& vertices = mesh.m_vertices;
    numberOfIndices  = indices.size();
    numberOfVertices = vertices.size();

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &vertexBuffer);
    checkBufferCreationError(vertexBuffer);
    glGenBuffers(1, &indexBuffer);
    checkBufferCreationError(indexBuffer);

    buffersInUseVector.push_back(vertexBuffer);

    glBindVertexArray(VAO);

    constexpr unsigned int dimension = 3;
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, dimension * sizeof(float) * numberOfVertices, vertices[0].data(), GL_STATIC_DRAW);

    // In Opengl there is only one index Buffer per VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numberOfIndices, &indices[0], GL_STATIC_DRAW);

    // Dimension of Vertex Data
    glVertexAttribPointer(vertexAttrIdx, dimension, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertexAttrIdx);

    // unbind
    glBindVertexArray(0);
}

void OpenGL3DDataObject::InitializeColorBuffer(const Color& color)
{
    if (colorBuffer == 0)
    {
        glGenBuffers(1, &colorBuffer);
        checkBufferCreationError(colorBuffer);
        buffersInUseVector.push_back(colorBuffer);
    }

    // Dimension of Color Data
    constexpr unsigned int dimension = 3;

    Eigen::Matrix<float, Eigen::Dynamic, dimension, Eigen::RowMajor> colorData;
    colorData = color.GetColor().replicate(1, numberOfVertices).transpose();
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorData.size(), colorData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(colorAttrIdx, dimension, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(colorAttrIdx);
    glBindVertexArray(0);
}

void OpenGL3DDataObject::InitializeTextureUVBuffer(const Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>& uvCoordinates)
{
    if (textureBuffer == 0)
    {
        glGenBuffers(1, &textureBuffer);
        checkBufferCreationError(textureBuffer);
        buffersInUseVector.push_back(textureBuffer);
    }

    // Dimension of Color Data
    constexpr unsigned int dimension = 2;

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvCoordinates.size(), uvCoordinates.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(textureAttrIdx, dimension, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(textureAttrIdx);
    glBindVertexArray(0);
}

void OpenGL3DDataObject::InitializeNormalBuffer(Mesh3D& mesh)
{
    if (mesh.HasNormals())
    {
        return;
    }
    if (normalsBuffer == 0)
    {
        glGenBuffers(1, &normalsBuffer);
        checkBufferCreationError(normalsBuffer);
        buffersInUseVector.push_back(normalsBuffer);
    }

    VertexNormalAttribute* pVertexNormalAttribute = dynamic_cast<VertexNormalAttribute*>(mesh.GetVertexAttribute(Mesh3D::EVertexAttribute::Normal));
    if (pVertexNormalAttribute == nullptr)
    {
        return;
    }
    // Dimension of Normals Data
    constexpr unsigned int dimension = 3;

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dimension * pVertexNormalAttribute->m_vertexNormals.size(), pVertexNormalAttribute->m_vertexNormals.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(normalAttrIdx, dimension, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(normalAttrIdx);
    glBindVertexArray(0);
}

void OpenGL3DDataObject::DrawObject(GLenum mode) const
{
    if (numberOfIndices == 0)
    {
        assert(false && "There are no vertices");
    }

    glBindVertexArray(VAO);
    glDrawElements(mode, numberOfIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
