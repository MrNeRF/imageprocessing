#include "Light.h"
#include "File.h"
#include "Macros.h"
#include "ObjFileParser.h"
#include <GL/glew.h>

void Light::Init(const std::string& pathToModel, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader)
{
    m_modelPath = pathToModel;
    m_spCamera  = spCamera;
    m_spShader  = spShader;

    m_spMesh3D = ObjFileParser().Parse(std::make_unique<File>(m_modelPath));
    if (m_spMesh3D != nullptr)
    {
        m_spOGLDataObject = std::make_unique<OpenGL3DDataObject>();
        m_spOGLDataObject->InitializeVertexBuffer(*m_spMesh3D);
        m_spOGLDataObject->InitializeColorBuffer(m_vertexColor);

        SetColor(m_vertexColor);
    }
    else
    {
        assert(0);
    }
}

void Light::SetColor(const Color& color)
{
    m_vertexColor = color;
    m_spOGLDataObject->InitializeColorBuffer(m_vertexColor);
}

void Light::Render()
{
    m_spShader->UseShader();
    m_spShader->SetVector("lightPosition", m_position);
    m_spShader->SetTransformationMatrix("view", m_spCamera->GetLookAt());
    m_spShader->SetTransformationMatrix("projection", m_spCamera->GetPerspectiveProjection());
    m_spOGLDataObject->DrawObject(GL_TRIANGLES);
}
