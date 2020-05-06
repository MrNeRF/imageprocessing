#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Color.h"
#include "Mesh3D.h"
#include <Eigen/Dense>
#include "OpenGL3DDataObject.h"
#include <memory>
#include <string>

class Object3D
{
	struct Transform
	{
		Eigen::Vector4f position{0.f,0.f,0.f,0.f};
		Eigen::Vector4f axis_angle{0.f, 1.f, 0.f, 0.f};
	};

public:
    Object3D(const std::string& pathToModel);
    void SetColor(const Color& color);
	void SetAxisAngle(Eigen::Vector3f axis, float angle) {m_transform.axis_angle = Eigen::Vector4f(axis.x(), axis.y(), axis.z(), angle);}
	const Eigen::Vector4f GetAxisAngle() {return m_transform.axis_angle;}
	const Eigen::Vector4f GetPosition() {return m_transform.position;}
	void SetPosition(Eigen::Vector4f pos) {m_transform.position = pos;}
    void Draw(void);

private:
    Transform m_transform;
    std::string path;
    std::unique_ptr<Mesh3D>             spMesh3D;
    std::unique_ptr<OpenGL3DDataObject> spOGLDataObject = std::make_unique<OpenGL3DDataObject>();


    Color vertexColor = Color(0.8f, 0.f, 0.f);
};

#endif
