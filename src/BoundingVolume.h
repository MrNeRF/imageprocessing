#ifndef BOUNDING_VOLUME_H
#define BOUNDING_VOLUME_H

#include "Camera.h"
#include "OpenGL3DDataObject.h"
#include "Shader.h"
#include <Eigen/Dense>
#include <memory>
#include "AABB.h"
#include "Mesh3D.h"
#include <vector>

class BoundingVolume
{
public:
    void init(std::shared_ptr<Camera> spCam, std::shared_ptr<Mesh3D> spMesh);
    bool IsBHHit(Eigen::Vector3f origin, Eigen::Vector3f direction, Eigen::Vector3f translate);
    void Draw(const Eigen::Matrix4f& model);

private:
    std::shared_ptr<OpenGL3DDataObject> m_spOGLDataObject;
    std::shared_ptr<Camera>             m_spCam;
    std::shared_ptr<Shader>             m_spShader;
	std::shared_ptr<Mesh3D>				m_spMesh3D;
    std::vector<Eigen::Vector3f>        m_vertices;
    std::vector<uint32_t>               m_indices;
	std::unique_ptr<AABB> m_spAABB;
};

#endif

