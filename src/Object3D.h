#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Camera.h"
#include "Color.h"
#include "Eigen/src/Geometry/AngleAxis.h"
#include "Eigen/src/Geometry/Quaternion.h"
#include "IEvent.h"
#include "IObserver.h"
#include "IRenderable.h"
#include "Light.h"
#include "Material.h"
#include "Mesh3D.h"
#include "OpenGL3DDataObject.h"
#include "Shader.h"
#include <Eigen/Dense>
#include <memory>
#include <string>

class Object3D : public IRenderable
    , public IObserver
{
public:
    const Eigen::Vector4f&    GetPosition() const { return m_position; }
    const Eigen::Quaternionf& GetOrientation() const { return m_orientation; }

    // IRenderable override
    void Init(const std::string& pathToModel, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader) override;
    void SetColor(const Color& color) override;
    void Render() override;

    void SetMaterial(std::shared_ptr<Material> spMaterial) { m_spMaterial = spMaterial; }
    void SetCamera(std::shared_ptr<Camera> spCamera) { m_spCamera = spCamera; }
    void SetLight(std::shared_ptr<Light> spLight) { m_spLight = spLight; }
    void UpdateOrientation(const Eigen::AngleAxisf& angleAxis) { m_orientation = Eigen::Quaternionf(angleAxis) * m_orientation; }
    void SetPosition(const Eigen::Vector4f& pos) { m_position = pos; }
    void ResetRotation() { m_orientation = Eigen::AngleAxis(0.f, Eigen::Vector3f::UnitX()); }

    // Observer overrides
    void onNotify(const EventType& eventType, IEvent* pEventData) override;	
private:
    std::string                         m_modelPath;
    Eigen::Vector4f                     m_position{0.f, 0.f, 0.f, 0.f};
    Eigen::Quaternionf                  m_orientation{Eigen::AngleAxis{0.f, Eigen::Vector3f::UnitX()}};
    Eigen::Vector2f                     m_dragAxis;
    std::unique_ptr<Mesh3D>             m_spMesh3D;
    std::unique_ptr<OpenGL3DDataObject> m_spOGLDataObject;
    std::shared_ptr<Camera>             m_spCamera;
    std::shared_ptr<Material>           m_spMaterial;
    std::shared_ptr<Shader>             m_spShader;
    std::shared_ptr<Light>              m_spLight;
    Color                               m_vertexColor = Color(0.8f, 0.f, 0.f);
};

#endif
