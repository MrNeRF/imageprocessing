#include "BoundingVolume.h"
#include <limits>

void BoundingVolume::init(std::shared_ptr<Camera> spCam, std::shared_ptr<Mesh3D> spMesh)
{
    m_spOGLDataObject = std::make_shared<OpenGL3DDataObject>();
    m_spCam           = spCam;
    m_spShader        = std::make_shared<Shader>("Line");
    m_spShader->InitShaders("../Shaders/color.vs", "../Shaders/color.fs");
    m_spMesh3D = spMesh;
	Eigen::Vector3f max = Eigen::Vector3f(std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest());
	Eigen::Vector3f min = Eigen::Vector3f(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());

    for (const auto& vertex : vertexIterator(*m_spMesh3D))
	{
		const auto& v = spMesh->GetVertex(vertex);
		max[0] = std::max(max[0], v[0]);
		max[1] = std::max(max[1], v[1]);
		max[2] = std::max(max[2], v[2]);

		min[0] = std::min(min[0], v[0]);
		min[1] = std::min(min[1], v[1]);
		min[2] = std::min(min[2], v[2]);
	}

	m_vertices.emplace_back(Eigen::Vector3f(min[0], max[1], min[2])); // FUL
	m_vertices.emplace_back(Eigen::Vector3f(max[0], max[1], min[2])); // FUR
	m_vertices.emplace_back(Eigen::Vector3f(max[0], min[1], min[2])); // FLR
	m_vertices.emplace_back(Eigen::Vector3f(min[0], min[1], min[2])); // FLL

	m_vertices.emplace_back(Eigen::Vector3f(min[0], max[1], max[2])); // BUL
	m_vertices.emplace_back(Eigen::Vector3f(max[0], max[1], max[2])); // BUR
	m_vertices.emplace_back(Eigen::Vector3f(max[0], min[1], max[2])); // BLR
	m_vertices.emplace_back(Eigen::Vector3f(min[0], min[1], max[2])); // BLL

	m_indices = {0, 1, 1, 2, 2, 3, 3, 0,
				 4, 5, 5, 6, 6, 7, 7, 4,
				 0, 4, 1, 5, 2, 6, 3, 7};


    m_spAABB = std::make_unique<AABB>(min, max);
    /* m_vertices = vertices; */
    /* m_indices  = indices; */
    m_spOGLDataObject->InitializeVertexBuffer(m_vertices, m_indices);
	std::vector<Eigen::Vector3f> colorData(m_vertices.size(), Color::GetColor(Color::EColor::YELLOW));
    m_spOGLDataObject->InitializeColorBuffer(colorData);
}

bool BoundingVolume::IsBHHit(Eigen::Vector3f origin, Eigen::Vector3f direction, Eigen::Vector3f translate)
{
	if(m_spAABB->IntersectRayAABB(origin, direction, translate).has_value())
	{
			return true;
	}
	else
	{
		return false;
	}
}

void BoundingVolume::Draw(const Eigen::Matrix4f& model)
{

    m_spShader->UseShader();
    m_spShader->SetTransformationMatrix("model", model);
    m_spShader->SetTransformationMatrix("view", m_spCam->GetLookAt());
    m_spShader->SetTransformationMatrix("projection", m_spCam->GetPerspectiveProjection());

    m_spOGLDataObject->DrawObject(GL_LINES);
}
