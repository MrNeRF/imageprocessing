#ifndef OPENLGL_3D_DATA_OBJECT_H
#define OPENLGL_3D_DATA_OBJECT_H

#include "Color.h"
#include "Eigen/Dense"
#include <GL/glew.h>
#include <vector>

class Mesh3D;

class OpenGL3DDataObject
{
public:
    OpenGL3DDataObject();
    ~OpenGL3DDataObject(void);
    void InitializeVertexBuffer(const std::vector<Eigen::Vector3f>& vertices, const std::vector<uint32_t>& indices);
    void InitializeVertexBuffer(Mesh3D& mesh);
    void InitializeNormalBuffer(Mesh3D& mesh);

    void InitializeColorBuffer(const std::vector<Eigen::Vector3f>& colors);
    void InitializeTextureUVBuffer(const Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>& uvCoordinates);
    void DrawObject(GLenum mode) const;

private:
    // Vertex Array Object
    unsigned int VAO = 0;
    // Vertex Element Object (EBO)
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
    unsigned int normalsBuffer = 0;
    unsigned int normalAttrIdx = 3;

    std::vector<unsigned int> buffersInUseVector;
    uint32_t                  m_numberOfIndices  = 0;
    uint32_t                  m_numberOfVertices = 0;
};

#endif
