#ifndef RAY_3D_H
#define RAY_3D_H

#include "Camera.h"
#include "OpenGL3DDataObject.h"
#include "Shader.h"
#include <Eigen/Dense>
#include <memory>
#include <vector>

class Ray3D
{
public:
    void init(std::shared_ptr<Camera> spCam, const std::vector<Eigen::Vector3f>& vertices, const std::vector<uint32_t> indices);
    void Draw();

private:
    std::shared_ptr<OpenGL3DDataObject> m_spOGLDataObject;
    std::shared_ptr<Camera>             m_spCam;
    std::shared_ptr<Shader>             m_spShader;
    std::vector<Eigen::Vector3f>        m_vertices;
    std::vector<uint32_t>               m_indices;
};

#endif
