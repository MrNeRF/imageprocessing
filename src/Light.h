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
    Light(const std::string name)
        : m_name{name}
    {
    }
    // IRenderable overrides
    void Init(std::shared_ptr<Mesh3D> spMesh3D, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader) override;
    void Render() override;

    const Color&              GetColor() { return m_vertexColor; }
    const Eigen::Vector4f&    GetPosition() const { return m_position; }
    const Eigen::Quaternionf& GetOrientation() const { return m_orientation; }
    void                      UpdateOrientation(const Eigen::AngleAxisf& angleAxis) { m_orientation = Eigen::Quaternionf(angleAxis) * m_orientation; }
    void                      SetPosition(const Eigen::Vector4f& pos) { m_position = pos; }

private:
    const std::string                   m_name;
    Eigen::Vector4f                     m_position = Eigen::Vector4f(0.f, 10.f, 5.f, 0.f);
    Eigen::Quaternionf                  m_orientation{Eigen::AngleAxis{0.f, Eigen::Vector3f::UnitX()}};
    std::shared_ptr<Mesh3D>             m_spMesh3D;
    std::unique_ptr<OpenGL3DDataObject> m_spOGLDataObject;
    std::shared_ptr<Camera>             m_spCamera;
    std::shared_ptr<Shader>             m_spShader;
    Color                               m_vertexColor = Color(1.f, 1.f, 1.f);
};

#endif
