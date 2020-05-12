#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "IRenderable.h"
#include "Mesh3D.h"
#include "OpenGL3DDataObject.h"
#include <Eigen/Dense>

class Light : public IRenderable
{
public:
    // IRenderable overrides
    void Init(const std::string& pathToModel, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader) override;
    void Render() override;
    void SetColor(const Color& color) override;

    const Color&           GetColor() { return m_vertexColor; }
    const Eigen::Vector4f&    GetPosition() const { return m_position; }
    const Eigen::Quaternionf& GetOrientation() const { return m_orientation; }
    void UpdateOrientation(const Eigen::AngleAxisf& angleAxis) { m_orientation = Eigen::Quaternionf(angleAxis) * m_orientation; }
    void SetPosition(const Eigen::Vector4f& pos) { m_position = pos; }

private:
    std::string                         m_modelPath;
    Eigen::Vector4f                     m_position = Eigen::Vector4f(0.f, 10.f, 5.f, 0.f);
    Eigen::Quaternionf                  m_orientation{Eigen::AngleAxis{0.f, Eigen::Vector3f::UnitX()}};
    std::unique_ptr<Mesh3D>             m_spMesh3D;
    std::unique_ptr<OpenGL3DDataObject> m_spOGLDataObject;
    std::shared_ptr<Camera>             m_spCamera;
    std::shared_ptr<Shader>             m_spShader;
    Color                               m_vertexColor = Color(1.f, 1.f, 1.f);
};

#endif
