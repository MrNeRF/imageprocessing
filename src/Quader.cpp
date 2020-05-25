#include "Quader.h"
#include "AABB.h"

std::shared_ptr<Mesh3D> Quader::CreateMesh()
{
    m_vertices.reserve(8);
    m_indices.reserve(36);

	float w = m_width * 0.5f;
	float h = m_height * 0.5f;
	float d = m_depth * 0.5f;
    
    // front vertices
    m_vertices.emplace_back(Eigen::Vector3f(-w,  h, -d)); // 0
    m_vertices.emplace_back(Eigen::Vector3f( w,  h, -d)); // 1
    m_vertices.emplace_back(Eigen::Vector3f( w, -h, -d)); // 2
    m_vertices.emplace_back(Eigen::Vector3f(-w, -h, -d)); // 3

    // back vertices
    m_vertices.emplace_back(Eigen::Vector3f( w,  h, d)); // 4
    m_vertices.emplace_back(Eigen::Vector3f(-w,  h, d)); // 5
    m_vertices.emplace_back(Eigen::Vector3f(-w, -h, d)); // 6
    m_vertices.emplace_back(Eigen::Vector3f( w, -h, d)); // 7

    auto createTriangleIndices =
        [& indices = m_indices](uint32_t upperLeft, uint32_t upperRight,
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
    auto createTriangleIndicesReverse =
        [& indices = m_indices](uint32_t upperLeft, uint32_t upperRight,
                                uint32_t lowerRight, uint32_t lowerLeft) 
        {
            // First triangle
            indices.push_back(upperRight);
            indices.push_back(lowerLeft);
            indices.push_back(upperLeft);

            // Second triangle
            indices.push_back(upperRight);
            indices.push_back(lowerRight);
            indices.push_back(lowerLeft);
        };
	
    //
    // indices

    // front
    createTriangleIndices(0, 1, 2, 3);
    // right
    createTriangleIndicesReverse(1, 4, 7, 2);
    // back
    createTriangleIndices(4, 5, 6, 7);
    // left
    createTriangleIndicesReverse(5, 0, 3, 6);
    // bottom
    createTriangleIndicesReverse(3, 2, 7, 6);
    // top
    createTriangleIndicesReverse(5, 4, 1, 0);

   return std::make_shared<Mesh3D>(m_vertices, m_indices, "Quader");
}


std::pair<bool, Eigen::Vector3f> Quader::IsHit(const Ray& rRay, Eigen::Vector3f position)
{
	if (auto intersection = m_aabb.IntersectRayAABB(rRay, position))
	{
		return std::pair(true, intersection.value().m_hitPoint);
	}

	return std::pair(false, Eigen::Vector3f());
}
