#include "Quader.h"

Quader::Quader(float width, float height, float depth) : m_width{width}, m_height{height}, m_depth{depth}
{
	m_vertices.reserve(8);
	m_indices.reserve(36);
	// front vertices 
	m_vertices.emplace_back(Eigen::Vector3f(-1.f, 1.f, -1.f));
	m_vertices.emplace_back(Eigen::Vector3f(1.f, 1.f, -1.f));
	m_vertices.emplace_back(Eigen::Vector3f(1.f, -1.f, -1.f));
	m_vertices.emplace_back(Eigen::Vector3f(-1.f, -1.f, -1.f));

	// back vertices
	m_vertices.emplace_back(Eigen::Vector3f(1.f, 1.f, 1.f));
	m_vertices.emplace_back(Eigen::Vector3f(-1.f, 1.f, 1.f));
	m_vertices.emplace_back(Eigen::Vector3f(-1.f, -1.f, 1.f));
	m_vertices.emplace_back(Eigen::Vector3f(1.f, -1.f, 1.f));

	auto createTriangleIndices = 
		[&indices = m_indices](uint32_t upperLeft, uint32_t upperRight, 
				uint32_t lowerRight, uint32_t lowerLeft)
		{
			// First triangle
			indices.push_back(upperLeft);
			indices.push_back(lowerRight);
			indices.push_back(lowerLeft);
			
			// Second triangle
			indices.push_back(lowerRight);
			indices.push_back(upperLeft);
			indices.push_back(upperRight);
		};
	// indices

	// front
	createTriangleIndices(0, 1, 2, 3);
	// right
	createTriangleIndices(1, 4, 7, 2);
	// back
	createTriangleIndices(4, 5, 6, 7);
	// left
	createTriangleIndices(5, 0, 3, 6);
	// top 
	createTriangleIndices(5, 4, 1, 0);
	// bottom
	createTriangleIndices(3, 2, 7, 6);
}


std::shared_ptr<Mesh3D> Quader::GetMesh()
{
	return std::make_shared<Mesh3D>(m_vertices, m_indices,"Quader");
}
