#include "Sphere.h"
#include "VertexNormalAttribute.h"
#include "Macros.h"

Sphere::Sphere(float radius, uint32_t stackCount, uint32_t sectorCount)
    : m_radius{}
    , m_stackCount{stackCount}
    , m_sectorCount{sectorCount}
{
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal

	float sectorStep = 2 * MathHelper::PI / sectorCount;
	float stackStep = MathHelper::PI / stackCount;
	float sectorAngle, stackAngle;

	for(uint32_t i = 0; i <= stackCount; ++i)
	{
		stackAngle = MathHelper::PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for(uint32_t j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			m_vertices.push_back(Eigen::Vector3f(x, y, z));

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			m_normals.push_back(Eigen::Vector3f(nx, ny, nz));
		}
	}

	int k1, k2;
	for(uint32_t i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for(uint32_t j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				m_indices.push_back(k1);
				m_indices.push_back(k2);
				m_indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if(i != (stackCount-1))
			{
				m_indices.push_back(k1 + 1);
				m_indices.push_back(k2);
				m_indices.push_back(k2 + 1);
			}
		}
	}

}

std::shared_ptr<Mesh3D> Sphere::GetMesh()
{
    auto spSphereMesh =  std::make_shared<Mesh3D>(m_vertices, m_indices, "Sphere");
    /* auto& vertexNormalAttrib = dynamic_cast<VertexNormalAttribute&>(spSphereMesh->AddVertexAttribute(Mesh3D::EVertexAttribute::Normal)); */
    /* vertexNormalAttrib.SetVertexNormals(m_normals); */
    return spSphereMesh;
}
