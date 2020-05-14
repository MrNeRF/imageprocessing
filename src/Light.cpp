#include "Light.h"
#include "File.h"
#include "Macros.h"
#include "Logger.h"
#include "ObjFileParser.h"
#include <GL/glew.h>

void Light::Init(std::shared_ptr<Mesh3D> spMesh3D, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader)
{
    m_spMesh3D = spMesh3D;
    m_spCamera  = spCamera;
    m_spShader  = spShader;

    if (m_spMesh3D != nullptr)
    {
        m_spOGLDataObject = std::make_unique<OpenGL3DDataObject>();
        m_spOGLDataObject->InitializeVertexBuffer(*m_spMesh3D);

        SetColor(m_vertexColor);
    }
    else
    {
        ASSERT(0);
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
    m_spShader->SetVector("transform.position", m_position);
    m_spShader->SetQuat("transform.qOrientation", m_orientation);
    m_spShader->SetQuat("transform.qconjOrientation", m_orientation.conjugate());
    m_spShader->SetTransformationMatrix("view", m_spCamera->GetLookAt());
    m_spShader->SetTransformationMatrix("projection", m_spCamera->GetPerspectiveProjection());
    m_spOGLDataObject->DrawObject(GL_TRIANGLES);
}
