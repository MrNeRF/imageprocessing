#include <GL/glew.h>
//
#include "AlgoVertexNormals.h"
#include "File.h"
#include "Light.h"
#include "Logger.h"
#include "Macros.h"
#include "ObjFileParser.h"
#include "VertexColorAttribute.h"
#include "VertexNormalAttribute.h"

void Light::Init(std::shared_ptr<Mesh3D> spMesh3D, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader)
{
    m_spMesh3D = spMesh3D;
    m_spCamera = spCamera;
    m_spShader = spShader;

    if (m_spMesh3D != nullptr)
    {
        m_spOGLDataObject                   = std::make_unique<OpenGL3DDataObject>();
        VertexColorAttribute&        rColor = dynamic_cast<VertexColorAttribute&>(m_spMesh3D->AddVertexAttribute(Mesh3D::EVertexAttribute::Color));
        std::vector<Eigen::Vector3f> colorData(m_spMesh3D->GetNumberOfVertice(), Color::GetColor(Color::EColor::WHITE));
        rColor.SetVertexColor(colorData, m_spMesh3D->GetIndices());
        auto pNormal = dynamic_cast<VertexNormalAttribute*>(m_spMesh3D->GetVertexAttribute(Mesh3D::EVertexAttribute::Normal));
        if (pNormal == nullptr)
        {
            AlgoVertexNormals algo(*m_spMesh3D, AlgoVertexNormals::NormalType::SMOOTH);
            ASSERT(algo.Compute() == true);
        }

        m_spOGLDataObject->InitializeBuffer(*m_spMesh3D);
    }
    else
    {
        ASSERT(0);
    }
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
