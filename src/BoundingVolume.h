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
#include "Ray.h"
#include "Primitives3D.h"

class BoundingVolume
{
public:
	enum class EBoundingVolume
	{
		Sphere,
		Cube
	};
public:
	BoundingVolume(EBoundingVolume type, const Mesh3D& rMeshToBound);

    bool IsBHHit(const Ray& rRay, Eigen::Vector3f translate);
    void Draw(const Eigen::Matrix4f& model, const Camera &rCam);
private:
	void createBoundingSphere(const Mesh3D& rMeshToBound);
	void createBoundingCube(const Mesh3D& rMeshToBound);

private:
	EBoundingVolume m_bvType;
    std::shared_ptr<OpenGL3DDataObject> m_spOGLDataObject;
    std::shared_ptr<Camera>             m_spCam;
    std::shared_ptr<Shader>             m_spShader;
	std::shared_ptr<Mesh3D>				m_spBVMesh;
	std::unique_ptr<Primitives3D> m_spBoundingVolume;
};

#endif

