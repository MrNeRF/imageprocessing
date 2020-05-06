#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Color.h"
#include "Eigen/src/Geometry/AngleAxis.h"
#include "Eigen/src/Geometry/Quaternion.h"
#include "Mesh3D.h"
#include "OpenGL3DDataObject.h"
#include <Eigen/Dense>
#include <memory>
#include <string>

class Object3D
{
public:
    Object3D(const std::string& pathToModel);
    void                      SetColor(const Color& color);
    const Eigen::Vector4f&    GetPosition() const { return position; }
    const Eigen::Quaternionf& GetOrientation() const { return orientation; }
    void                      UpdateOrientation(const Eigen::AngleAxisf& angleAxis) { orientation = orientation * Eigen::Quaternionf(angleAxis); }
    void                      SetPosition(const Eigen::Vector4f& pos) { position = pos; }
    void                      Draw(void);

private:
    Eigen::Vector4f                     position{0.f, 0.f, 0.f, 0.f};
    Eigen::Quaternionf                  orientation{Eigen::AngleAxis{0.f, Eigen::Vector3f::UnitX()}};
    std::string                         path;
    std::unique_ptr<Mesh3D>             spMesh3D;
    std::unique_ptr<OpenGL3DDataObject> spOGLDataObject = std::make_unique<OpenGL3DDataObject>();

    Color vertexColor = Color(0.8f, 0.f, 0.f);
};

#endif
