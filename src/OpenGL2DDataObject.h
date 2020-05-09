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
    void CreateColor(const Color& color);
    void CreateTextureCoordinates(const DataTypeVertices& uvCoordinates);
    void DrawObject(GLenum mode) const;

private:
    // Vertex Array Object
    unsigned int VAO = 0;

    // Vertex Element Object
    unsigned int indexBuffer = 0;

    // Vertex Buffer
    unsigned int vertexBuffer  = 0;
    unsigned int vertexAttrIdx = 0;

    // Color Buffer
    unsigned int colorBuffer  = 0;
    unsigned int colorAttrIdx = 1;

    // Texture Buffer
    unsigned int textureBuffer  = 0;
    unsigned int textureAttrIdx = 2;

    // Normal Buffer
    unsigned int normalsVBO    = 0;
    unsigned int normalAttrIdx = 3;

    unsigned int numberOfIndices  = 0;
    unsigned int numberOfVertices = 0;

    std::vector<unsigned int> buffersInUseVector;
};

#endif
