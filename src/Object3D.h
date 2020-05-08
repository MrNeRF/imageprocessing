#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Color.h"
#include "Eigen/src/Geometry/AngleAxis.h"
#include "Eigen/src/Geometry/Quaternion.h"
#include "Mesh3D.h"
#include "OpenGL3DDataObject.h"
#include "IObserver.h"
#include <Eigen/Dense>
#include "IEvent.h"
#include <memory>
#include <string>

class Object3D : public IObserver
{
public:
    Object3D(const std::string& pathToModel);
    void                      SetColor(const Color& color);
    const Eigen::Vector4f&    GetPosition() const { return position; }
    const Eigen::Quaternionf& GetOrientation() const { return orientation; }
    void                      UpdateOrientation(const Eigen::AngleAxisf& angleAxis) { orientation = Eigen::Quaternionf(angleAxis) * orientation   ; }
    void                      SetPosition(const Eigen::Vector4f& pos) { position = pos; }
    void					  ResetRotation() {orientation = Eigen::AngleAxis(0.f, Eigen::Vector3f::UnitX());}
    void                      Draw(void);

	// Observer
	void onNotify(const EventType& eventType, IEvent* pEventData) override;	
private:
    Eigen::Vector4f                     position{0.f, 0.f, 0.f, 0.f};
    Eigen::Quaternionf                  orientation{Eigen::AngleAxis{0.f, Eigen::Vector3f::UnitX()}};
	Eigen::Vector2f dragAxis;
    std::string                         path;
    std::unique_ptr<Mesh3D>             spMesh3D;
    std::unique_ptr<OpenGL3DDataObject> spOGLDataObject = std::make_unique<OpenGL3DDataObject>();

    Color vertexColor = Color(0.8f, 0.f, 0.f);
};

#endif
