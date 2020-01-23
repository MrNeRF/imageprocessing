#include "HalfEdgeDS.h"
#include <cassert>

void HalfEdgeDS::Initialize(Mesh3D* meshData)
{
    if (meshData == nullptr)
    {
        assert(meshData != nullptr && "Meshdata is empty");
        return;
    }

    // facecount ist indices / 3
    int faceCount = meshData->indices.size();

    // die indices liefern die Position der Vertices.
    for (int i = 0; i < faceCount; i += 3)
    {
        const Eigen::Vector3f& v0 = meshData->vertices.block(i, 0, 1, 3).transpose();
        const Eigen::Vector3f& v1 = meshData->vertices.block(i + 1, 0, 1, 3).transpose();
        const Eigen::Vector3f& v2 = meshData->vertices.block(i + 2, 0, 1, 3).transpose();

        Eigen::Vector3f faceNormal = meshData->normals.block(i, 0, 1, 3).transpose() + meshData->vertices.block(i + 1, 0, 1, 3).transpose() + meshData->vertices.block(i + 2, 0, 1, 3).transpose();
        faceNormal.normalize();
        faces.emplace_back(std::make_shared<Face>(i));
        // We need to take the dot product between the face normal and the boundary sides of the triangle to determine
        // the correct winding order.
        // (from, to)
        if (faceNormal.dot((v1 - v0).cross(v2 - v0)) >= 0)
        {
            /* edge0 = v1 - v0; */
            const std::pair<int, int> e0 = std::make_pair(i, i + 1);
            /* edge1 = v2 - v1; */
            const std::pair<int, int> e1 = std::make_pair(i + 1, i + 2);
            /* edge2 = v0 - v2; */
            const std::pair<int, int> e2 = std::make_pair(i + 2, i);

            auto vertex0 = std::make_shared<Vertex>(i, v0);
            auto vertex1 = std::make_shared<Vertex>(i + 1, v1);
            auto vertex2 = std::make_shared<Vertex>(i + 2, v2);

            fillDataByFace(e0, e1, e2, vertex0, vertex1, vertex2);
        }
        else
        {
            /* edge0 = v2 - v0; */
            const std::pair<int, int> e0 = std::make_pair(i, i + 2);
            /* edge1 = v1 - v2; */
            const std::pair<int, int> e1 = std::make_pair(i + 2, i + 1);
            /* edge2 = v0 - v1; */
            const std::pair<int, int> e2 = std::make_pair(i + 1, i);

            auto vertex0 = std::make_shared<Vertex>(i, v0);
            auto vertex1 = std::make_shared<Vertex>(i + 2, v2);
            auto vertex2 = std::make_shared<Vertex>(i + 1, v1);

            fillDataByFace(e0, e1, e2, vertex0, vertex1, vertex2);
        }
    }
}

void HalfEdgeDS::fillDataByFace(const std::pair<int, int>& e0,
                                const std::pair<int, int>& e1,
                                const std::pair<int, int>& e2,
                                std::shared_ptr<Vertex>    vertex0,
                                std::shared_ptr<Vertex>    vertex1,
                                std::shared_ptr<Vertex>    vertex2)
{
    edges[e0]                    = std::make_shared<HalfEdge>();
    vertex0->wspOutgoingHalfEdge = edges[e0];
    vertices.push_back(vertex0);
    edges[e0]->spFace = faces.back();

    edges[e1]                    = std::make_shared<HalfEdge>();
    vertex1->wspOutgoingHalfEdge = edges[e1];
    vertices.push_back(vertex1);
    edges[e1]->spFace = faces.back();

    edges[e2]                    = std::make_shared<HalfEdge>();
    vertex2->wspOutgoingHalfEdge = edges[e2];
    vertices.push_back(vertex2);
    edges[e2]->spFace = faces.back();

    faces.back()->wspBoundingHalfEdge = edges[e0];

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

