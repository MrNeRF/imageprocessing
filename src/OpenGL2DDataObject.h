#ifndef OPENLGL_2D_DATA_OBJECT_H
#define OPENLGL_2D_DATA_OBJECT_H

#include "Color.h"
#include "Eigen/Dense"
#include <GL/glew.h>
#include <vector>

using DataTypeVertices = Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>;

class OpenGL2DDataObject
{
public:
    OpenGL2DDataObject() { glGenVertexArrays(1, &VAO); }
    ~OpenGL2DDataObject(void);
    void CreateVertices(const DataTypeVertices& vertices, std::vector<int> indices);
    void CreateColor(const Color& rgb, std::vector<int> indices);
    void DrawObject(GLenum mode) const;

private:
    // Vertex Array Object
    unsigned int VAO = 0;
    // Vertex and Element Buffer Object for Coordinate Attribute
    unsigned int verticesVBO = 0;
    unsigned int verticesEBO = 0;
    unsigned int vertexAttribIndex = 0;
    // Vertex and Element Buffer Object for Coordinate Attribute
    unsigned int textureVBO = 0;
    unsigned int textureEBO = 0;
    unsigned int textureAttribIndex = 1;
    // Vertex and Element Buffer Object for Coordinate Attribute
    unsigned int normalsVBO = 0;
    unsigned int normalsEBO = 0;
    unsigned int normalsAttribIndex = 1;
    // Vertex and Element Buffer Object for Coordinate Attribute
    unsigned int colorsVBO = 0;
    unsigned int colorsEBO = 0;
    unsigned int colorsAttribIndex = 1;

    unsigned numberOfVertices = 0;
    std::vector<unsigned int> buffersBO;
};

#endif
