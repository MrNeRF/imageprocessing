#include "BoundingVolume.h"
#include "Quader.h"
#include <limits>

BoundingVolume::BoundingVolume(BoundingVolume::EBoundingVolume type, const Mesh3D& rMeshToBound)
	: m_bvType{type}
{
    m_spShader        = std::make_shared<Shader>("Line");
    m_spShader->InitShaders("../Shaders/color.vs", "../Shaders/color.fs");
	m_spOGLDataObject = std::make_shared<OpenGL3DDataObject>();
	switch(type)
	{
		case BoundingVolume::EBoundingVolume::Sphere:
			{
				createBoundingSphere(rMeshToBound);
			}
			break;
		case BoundingVolume::EBoundingVolume::Cube:
			{
				createBoundingCube(rMeshToBound);
			}
			break;
	}
}

void BoundingVolume::createBoundingCube(const Mesh3D& rMeshToBound)
{
	Eigen::Vector3f max = Eigen::Vector3f(std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest());
	Eigen::Vector3f min = Eigen::Vector3f(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());

    for (const auto& vertex : vertexIterator(rMeshToBound))
	{
		const auto& v = rMeshToBound.GetVertex(vertex);
		max[0] = std::max(max[0], v[0]);
		max[1] = std::max(max[1], v[1]);
		max[2] = std::max(max[2], v[2]);

		min[0] = std::min(min[0], v[0]);
		min[1] = std::min(min[1], v[1]);
		min[2] = std::min(min[2], v[2]);
	}


    m_spBoundingVolume = std::make_unique<Quader>(min, max);
    m_spBVMesh = m_spBoundingVolume->CreateMesh();

    m_spOGLDataObject->InitializeVertexBuffer(m_spBVMesh->GetVertices(), m_spBVMesh->GetIndices());
	std::vector<Eigen::Vector3f> colorData(m_spBVMesh->GetNumberOfVertice(), Color::GetColor(Color::EColor::YELLOW));
    m_spOGLDataObject->InitializeColorBuffer(colorData);
		
}
void BoundingVolume::createBoundingSphere(const Mesh3D& rMeshToBound)
{

}


bool BoundingVolume::IsBHHit(const Ray& rRay, Eigen::Vector3f translate)
{
	switch(m_bvType)
	{
		case EBoundingVolume::Cube:
		{
			Quader* pCube = static_cast<Quader*>(m_spBoundingVolume.get());
			auto[bHit, hitPoint] = pCube->IsHit(rRay,translate);
			return bHit;
		}
		break;
		case EBoundingVolume::Sphere:
		{

		}

	}

	return false;
}

void BoundingVolume::Draw(const Eigen::Matrix4f& model, const Camera &rCam) 
{

    m_spShader->UseShader();
    m_spShader->SetTransformationMatrix("model", model);
    m_spShader->SetTransformationMatrix("view", rCam.GetLookAt());
    m_spShader->SetTransformationMatrix("projection", rCam.GetPerspectiveProjection());

    m_spOGLDataObject->DrawObject(GL_TRIANGLES);
}
