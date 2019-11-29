#include "OpenGL2DDataObject.h"
#include <cassert>

static void checkBufferCreationError(unsigned int bufferObjectID)
{
    if (bufferObjectID == 0)
    {
        assert(false && "Creating Buffer Object failed!\n");
    }
}

OpenGL2DDataObject::~OpenGL2DDataObject(void)
{
    // Delete Array Buffers.
    glDeleteBuffers(buffersInUseVector.size(), &buffersInUseVector[0]);
    // Delete Indexbuffer
    glDeleteBuffers(1, &indexBuffer);
}

void OpenGL2DDataObject::CreateVertices(const DataTypeVertices& vertices, std::vector<int> indices)
{
    if (indices.empty() || vertices.size() == 0)
    {
        assert(false && "There are no vertices indexed");
    }

    numberOfIndices  = indices.size();
    numberOfVertices = vertices.size();

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &vertexBuffer);
    checkBufferCreationError(vertexBuffer);
    glGenBuffers(1, &indexBuffer);
    checkBufferCreationError(indexBuffer);

    buffersInUseVector.push_back(vertexBuffer);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfVertices, vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numberOfIndices, &indices[0], GL_STATIC_DRAW);

    // Dimension of Vertex Data
    constexpr unsigned int dimension = 2;
    glVertexAttribPointer(vertexAttrIdx, dimension, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertexAttrIdx);

    // unbind
    glBindVertexArray(0);
}

void OpenGL2DDataObject::CreateColor(const Color& color)
{
    glGenBuffers(1, &colorBuffer);
    checkBufferCreationError(colorBuffer);
    buffersInUseVector.push_back(colorBuffer);

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

void OpenGL2DDataObject::DrawObject(GLenum mode) const
{
    if (numberOfIndices == 0)
    {
        assert(false && "There are no vertices");
    }

    glBindVertexArray(VAO);
    glDrawElements(mode, numberOfIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
