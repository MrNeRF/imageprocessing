#include "OpenGL3DDataObject.h"
#include <cassert>

void OpenGL3DDataObject::CreateVertices(const DataTypeVertices& vertices, std::vector<int> indices)
{
    numberOfVertices = indices.size();
    if (numberOfVertices < 1)
    {
        assert(false && "There are no vertices");
    }

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &verticesVBO);
    glGenBuffers(1, &verticesEBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, verticesEBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
}

void OpenGL3DDataObject::DrawObject(GLenum mode) const
{
    if (numberOfVertices < 1)
    {
        assert(false && "There are no vertices");
    }

    glBindVertexArray(VAO);
    glDrawElements(mode, numberOfVertices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
