#include "Ray3D.h"
#include "Color.h"


void Ray3D::init(std::shared_ptr<Camera> spCam,const std::vector<Eigen::Vector3f>& vertices, const std::vector<uint32_t> indices)
{
	m_spOGLDataObject = std::make_shared<OpenGL3DDataObject>();
	m_spCam = spCam;
	m_spShader = std::make_shared<Shader>("Line");
	m_spShader->InitShaders("../Shaders/color.vs", "../Shaders/color.fs");
	m_vertices = vertices;
	m_indices = indices;
}

void Ray3D::Draw()
{	
	m_spOGLDataObject->InitializeVertexBuffer(m_vertices, m_indices);
	m_spOGLDataObject->InitializeColorBuffer(Color(0.f, 1.f, 0.f));

    m_spShader->UseShader();

    m_spShader->SetTransformationMatrix("view", m_spCam->GetLookAt());
    m_spShader->SetTransformationMatrix("projection", m_spCam->GetPerspectiveProjection());

    m_spOGLDataObject->DrawObject(GL_LINES);
}
