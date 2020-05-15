#include "Mesh3D.h"
#include "VertexColorAttribute.h"
#include "VertexNormalAttribute.h"
#include "Logger.h"
#include <iostream>
#include <typeinfo>

Mesh3D::Mesh3D(const std::vector<Eigen::Vector3f>& vertexData,
               const std::vector<uint32_t>&        indexVector,
               const std::string& name)
    : m_name{name}
	, m_indices{indexVector}
    , m_vertices(vertexData)
    , m_halfEdgeDS(*this)
{
    if (m_indices.empty() || vertexData.size() == 0)
    {
		Logger::GetInstance().GetLogger().error("There are no vertices indexed");
		ASSERT(0);
    }
    m_halfEdgeDS.InitHalfEdgeDS();
}

VertexAttribute& Mesh3D::AddVertexAttribute(EVertexAttribute vertexAttribute)
{
    switch (vertexAttribute)
    {
        case EVertexAttribute::Normal:
            m_vertexAttributes.emplace_back(std::make_unique<VertexNormalAttribute>(m_vertices, m_indices));
            break;
        case EVertexAttribute::Color:
            m_vertexAttributes.emplace_back(std::make_unique<VertexColorAttribute>(m_vertices, m_indices));
            break;
        case EVertexAttribute::UVCoordinates:
            break;
            /* default: */
            /* return nullptr; */
    }

    return *(m_vertexAttributes.back());
}

VertexAttribute* Mesh3D::GetVertexAttribute(EVertexAttribute vertexAttribute)
{
    switch (vertexAttribute)
    {
        case EVertexAttribute::Normal:
            for (auto const& attribute : m_vertexAttributes)
            {
                auto pAttri = attribute.get();
                if (typeid(*pAttri) == typeid(VertexNormalAttribute))
                {
                    return attribute.get();
                }
            }
            return nullptr;
            break;
        case EVertexAttribute::Color:
            for (auto const& attribute : m_vertexAttributes)
            {
                auto pAttri = attribute.get();
                if (typeid(*pAttri) == typeid(VertexColorAttribute))
                {
                    return attribute.get();
                }
            }
            return nullptr;
            break;
        case EVertexAttribute::UVCoordinates:
            /* for (auto const& attribute : m_vertexAttributes) */
            /* { */
            /* 	if (typeid(*attribute)	 == typeid(Vertex)) */
            /* 	{ */
            /* 		return attribute.get(); */
            /* 	} */
            /* } */
            return nullptr;
            break;
        default:
            return nullptr;
    }
}

TriangleAttribute* Mesh3D::GetTriangleAttribute(ETriangleAttribute triangleAttribute)
{
    return nullptr;
}

void Mesh3D::HalfEdgeDS::InitHalfEdgeDS(void)
{
    // faceCount ist indices / 3;
    uint32_t faceCount = static_cast<uint32_t>(static_cast<float>(m_mesh.m_indices.size()) / 3.f);
    faces.resize(faceCount);
    vertices.resize(m_mesh.m_vertices.size());
    uint32_t vIdx = 0;
    for (uint32_t faceIdx = 0; faceIdx < faceCount; ++faceIdx)
    {
        // Get indices for the three face vertices
        int i0 = m_mesh.m_indices[vIdx++];
        int i1 = m_mesh.m_indices[vIdx++];
        int i2 = m_mesh.m_indices[vIdx++];

		// Get the vertices
        const Eigen::Vector3f& v0 = m_mesh.m_vertices[i0];
        const Eigen::Vector3f& v1 = m_mesh.m_vertices[i1];
        const Eigen::Vector3f& v2 = m_mesh.m_vertices[i2];

        faces[faceIdx] = std::make_shared<Face>(faceIdx);
        // @TODO
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

    std::pair<int, int> oppositeHalfEdge0{e0.second, e0.first};
    std::pair<int, int> oppositeHalfEdge1{e1.second, e1.first};
    std::pair<int, int> oppositeHalfEdge2{e2.second, e2.first};

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

void Mesh3D::IterateAllFaces() const
{
	for(const auto spFace: m_halfEdgeDS.faces)
	{
		auto spEdgeBegin = spFace->wspBoundingHalfEdge.lock();
		ASSERT(spEdgeBegin != nullptr)	
		auto spEdgeNext = spEdgeBegin;
		std::cout << "Face " << spFace->id << "\n";
		do
		{
			ASSERT(spEdgeNext != nullptr)
			const Eigen::Vector3f& point = spEdgeNext->spDestinationVertex->position;
			std::cout << "Vector: (" << point.x() << ", " << point.y() << ", " << point.z() << ")\n";
			spEdgeNext = spEdgeNext->wspNextHalfeEdge.lock();
		}
		while(spEdgeBegin != spEdgeNext);
	}

	// Vertices
	/* std::cout << "Vertices \n"; */
	/* auto iter = m_vertices.begin(); */ 
	/* for(const auto& v : m_halfEdgeDS.vertices) */
	/* { */
	/* 	std::cout << "HalfEdge: (" << v->position.x() << ", " << v->position.y() << ", " << v->position.z() << ")\n"; */
	/* 	std::cout << "Mesh    : (" << (*iter).x() << ", " << (*iter).y() << ", " << (*iter).z() << ")\n"; */
	/* 	iter = std::next(iter); */
	/* } */
	
}
