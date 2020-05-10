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
    const Eigen::Vector3f& GetPosition() { return m_position; }

private:
    std::string                         m_modelPath;
    Eigen::Vector3f                     m_position{10.f, 10.f, 0.f};
    std::unique_ptr<Mesh3D>             m_spMesh3D;
    std::unique_ptr<OpenGL3DDataObject> m_spOGLDataObject;
    std::shared_ptr<Shader>             m_spShader;
    std::shared_ptr<Camera>             m_spCamera;
    Color                               m_vertexColor = Color(1.f, 1.f, 1.f);
};

#endif
