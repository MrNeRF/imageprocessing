#include "Mesh3D.h"
#include <cassert>
#include <iostream>

Mesh3D::Mesh3D(const Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>& vertexData,
               const std::vector<uint32_t>&                                    indexVector)
    : indices{indexVector}
    , vertices(vertexData)
    , m_halfEdgeDS(*this)
{
    if (indices.empty() || vertexData.size() == 0)
    {
        assert(false && "There are no vertices indexed");
    }
    m_halfEdgeDS.InitHalfEdgeDS();
}

void Mesh3D::HalfEdgeDS::InitHalfEdgeDS(void)
{
    // faceCount ist indices / 3;
    uint32_t faceCount = static_cast<uint32_t>(static_cast<float>(m_mesh.indices.size()) / 3.f);
    faces.resize(faceCount);
    vertices.resize(m_mesh.indices.size());
    uint32_t vIdx = 0;
    for (uint32_t faceIdx = 0; faceIdx < faceCount; ++faceIdx)
    {
        int i0 = m_mesh.indices[vIdx++];
        int i1 = m_mesh.indices[vIdx++];
        int i2 = m_mesh.indices[vIdx++];

        const Eigen::Vector3f& v0 = m_mesh.vertices.block(i0, 0, 1, 3).transpose();
        const Eigen::Vector3f& v1 = m_mesh.vertices.block(i1, 0, 1, 3).transpose();
        const Eigen::Vector3f& v2 = m_mesh.vertices.block(i2, 0, 1, 3).transpose();

        faces[faceIdx] = std::make_shared<Face>(faceIdx);
        // We need to take the dot product between the face normal and the boundary sides of the triangle to determine
        // the correct winding order.
        // (from, to)
        /* edge0 = v1 - v0; */
        const std::pair<int, int> e0 = std::make_pair(i0, i1);
        /* edge1 = v2 - v1; */
        const std::pair<int, int> e1 = std::make_pair(i1, i2);
        /* edge2 = v0 - v2; */
        const std::pair<int, int> e2 = std::make_pair(i2, i0);

        auto vertex0 = vertices[i0];
        if (vertex0 == nullptr)
        {
            vertex0      = std::make_shared<Vertex>(i0, v0);
            vertices[i0] = vertex0;
        }

        auto vertex1 = vertices[i1];
        if (vertex1 == nullptr)
        {
            vertex1      = std::make_shared<Vertex>(i1, v1);
            vertices[i1] = vertex1;
        }

        auto vertex2 = vertices[i2];
        if (vertex2 == nullptr)
        {
            vertex2      = std::make_shared<Vertex>(i2, v2);
            vertices[i2] = vertex2;
        }

        createFace(e0, e1, e2, vertex0, vertex1, vertex2, faceIdx);
    }
}

void Mesh3D::HalfEdgeDS::createFace(const std::pair<int, int>& e0,
                                    const std::pair<int, int>& e1,
                                    const std::pair<int, int>& e2,
                                    std::shared_ptr<Vertex>    vertex0,
                                    std::shared_ptr<Vertex>    vertex1,
                                    std::shared_ptr<Vertex>    vertex2,
                                    uint32_t                   faceIdx)
{
    edges[e0]                    = std::make_shared<HalfEdge>();
    vertex0->wspOutgoingHalfEdge = edges[e0];
    edges[e0]->spFace            = faces[faceIdx];

    edges[e1]                    = std::make_shared<HalfEdge>();
    vertex1->wspOutgoingHalfEdge = edges[e1];
    edges[e1]->spFace            = faces[faceIdx];

    edges[e2]                    = std::make_shared<HalfEdge>();
    vertex2->wspOutgoingHalfEdge = edges[e2];
    edges[e2]->spFace            = faces[faceIdx];

    faces[faceIdx]->wspBoundingHalfEdge = edges[e0];

    edges[e0]->spDestinationVertex = vertex1;
    edges[e1]->spDestinationVertex = vertex2;
    edges[e2]->spDestinationVertex = vertex0;

    edges[e0]->wspNextHalfeEdge     = edges[e1];
    edges[e0]->wspPreviousHalfeEdge = edges[e2];

    edges[e1]->wspNextHalfeEdge     = edges[e2];
    edges[e1]->wspPreviousHalfeEdge = edges[e0];

    edges[e2]->wspNextHalfeEdge     = edges[e0];
    edges[e2]->wspPreviousHalfeEdge = edges[e1];

    std::pair<int, int> oppositeHalfEdge0 = std::make_pair(e0.second, e0.first);
    std::pair<int, int> oppositeHalfEdge1 = std::make_pair(e1.second, e1.first);
    std::pair<int, int> oppositeHalfEdge2 = std::make_pair(e2.second, e2.first);

    if (edges[oppositeHalfEdge0] != nullptr)
    {
        edges[e0]->wspOppositeHalfeEdge                = edges[oppositeHalfEdge0];
        edges[oppositeHalfEdge0]->wspOppositeHalfeEdge = edges[e0];
    }
    if (edges[oppositeHalfEdge1] != nullptr)
    {
        edges[e1]->wspOppositeHalfeEdge                = edges[oppositeHalfEdge1];
        edges[oppositeHalfEdge1]->wspOppositeHalfeEdge = edges[e1];
    }
    if (edges[oppositeHalfEdge2] != nullptr)
    {
        edges[e2]->wspOppositeHalfeEdge                = edges[oppositeHalfEdge2];
        edges[oppositeHalfEdge2]->wspOppositeHalfeEdge = edges[e2];
    }
}
