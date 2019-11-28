#ifndef OPENLGL_3D_DATA_OBJECT_H
#define OPENLGL_3D_DATA_OBJECT_H

#include "Eigen/Dense"
#include <GL/glew.h>
#include <vector>

using DataTypeVertices = Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>;

class OpenGL3DDataObject
{
public:
    void CreateVertices(const DataTypeVertices& vertices, std::vector<int> indices);
    void DrawObject(GLenum mode) const;

private:
    // Vertex Array Object
    unsigned int VAO = 0;
    // Vertex and Element Buffer Object for Coordinate Attribute
    unsigned int verticesVBO = 0;
    unsigned int verticesEBO = 0;
    // Vertex and Element Buffer Object for Coordinate Attribute
    unsigned int textureVBO = 0;
    unsigned int textureEBO = 0;
    // Vertex and Element Buffer Object for Coordinate Attribute
    unsigned int normalsVBO = 0;
    unsigned int normalsEBO = 0;
    // Vertex and Element Buffer Object for Coordinate Attribute
    unsigned int colorsVBO = 0;
    unsigned int colorsEBO = 0;

    unsigned numberOfVertices = 0;
};

#endif
