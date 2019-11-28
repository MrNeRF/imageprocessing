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
    glDeleteBuffers(buffersBO.size(), &buffersBO[0]);
}

void OpenGL2DDataObject::CreateVertices(const DataTypeVertices& vertices, std::vector<int> indices)
{
    numberOfVertices = indices.size();
    if (numberOfVertices < 1)
    {
        assert(false && "There are no vertices");
    }

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &verticesVBO);
    checkBufferCreationError(verticesVBO);
    glGenBuffers(1, &verticesEBO);
    checkBufferCreationError(verticesEBO);

    buffersBO.push_back(verticesVBO);
    buffersBO.push_back(verticesEBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, verticesEBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(vertexAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertexAttribIndex);
}

void OpenGL2DDataObject::CreateColor(const Color& color, std::vector<int> indices)
{
    numberOfVertices = indices.size();
    if (numberOfVertices < 1)
    {
        assert(false && "There are no colors");
    }

    glGenBuffers(1, &colorsVBO);
    checkBufferCreationError(colorsVBO);
    glGenBuffers(1, &colorsEBO);
    checkBufferCreationError(colorsEBO);

    buffersBO.push_back(colorsVBO);
    buffersBO.push_back(colorsEBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, color.GetColor().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, colorsEBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(colorsAttribIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(colorsAttribIndex);
}

void OpenGL2DDataObject::DrawObject(GLenum mode) const
{
    if (numberOfVertices < 1)
    {
        assert(false && "There are no vertices");
    }

    glBindVertexArray(VAO);
    glDrawElements(mode, numberOfVertices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
