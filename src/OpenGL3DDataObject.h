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
    void InitializeBuffer(Mesh3D& mesh);
    void DrawObject(GLenum mode) const;

private:
    // Vertex Array Object
    unsigned int m_VAO               = 0;
    unsigned int m_VBO               = 0;
    uint32_t     m_vertexRenderCount = 0;
};

#endif
