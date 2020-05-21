#include "Ray3D.h"
#include "Color.h"

void Ray3D::init(std::shared_ptr<Camera> spCam, const std::vector<Eigen::Vector3f>& vertices, const std::vector<uint32_t> indices)
{
    m_spOGLDataObject = std::make_shared<OpenGL3DDataObject>();
    m_spCam           = spCam;
    m_spShader        = std::make_shared<Shader>("Line");
    m_spShader->InitShaders("../Shaders/color.vs", "../Shaders/color.fs");
    m_vertices = vertices;
    m_indices  = indices;
    m_spOGLDataObject->InitializeVertexBuffer(m_vertices, m_indices);
    m_spOGLDataObject->InitializeColorBuffer({Color::GetColor(Color::EColor::RED), Color::GetColor(Color::EColor::GREEN)});
}

void Ray3D::Draw()
{
    m_spShader->UseShader();

	Eigen::Matrix4f identity = Eigen::Matrix4f::Identity();
    m_spShader->SetTransformationMatrix("model", identity);
    m_spShader->SetTransformationMatrix("view", m_spCam->GetLookAt());
    m_spShader->SetTransformationMatrix("projection", m_spCam->GetPerspectiveProjection());

    m_spOGLDataObject->DrawObject(GL_LINES);
}
