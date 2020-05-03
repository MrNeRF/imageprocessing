#ifndef MESH_3D_H
#define MESH_3D_H

#include "TriangleAttribute.h"
#include "VertexAttribute.h"
#include <Eigen/Dense>
#include <map>
#include <memory>
#include <utility>
#include <vector>

using Vertex = uint32_t*;

class Mesh3D
{
public:
    enum class EVertexAttribute
    {
        Normal,
        Color,
        UVCoordinates
    };

    enum class ETriangleAttribute
    {
        Normal,
        Color
    };

private:
    class HalfEdgeDS
    {
    private:
        struct HalfEdge;

        struct Vertex
        {
            Vertex(int vertexID, const Eigen::Vector3f& pos)
                : id{vertexID}
                , position{pos} {};

            int                     id;
            const Eigen::Vector3f&  position;
            std::weak_ptr<HalfEdge> wspOutgoingHalfEdge;
        };

        struct Face
        {
            Face(int faceID)
                : id{faceID} {};
            int                     id;
            std::weak_ptr<HalfEdge> wspBoundingHalfEdge;
        };

        struct HalfEdge
        {
            std::weak_ptr<HalfEdge> wspOppositeHalfeEdge;
            std::weak_ptr<HalfEdge> wspNextHalfeEdge;
            std::weak_ptr<HalfEdge> wspPreviousHalfeEdge;

            std::shared_ptr<Vertex> spDestinationVertex;
            std::shared_ptr<Face>   spFace;
        };

    public:
        HalfEdgeDS(Mesh3D& mesh)
            : m_mesh(mesh){};

        void InitHalfEdgeDS(void);

    private:
        void createFace(const std::pair<int, int>& e0,
                        const std::pair<int, int>& e1,
                        const std::pair<int, int>& e2,
                        std::shared_ptr<Vertex>    vertex0,
                        std::shared_ptr<Vertex>    vertex1,
                        std::shared_ptr<Vertex>    vertex2,
                        uint32_t                   faceIdx);

    private:
        Mesh3D&                                                  m_mesh;
        std::map<std::pair<int, int>, std::shared_ptr<HalfEdge>> edges;
        std::vector<std::shared_ptr<Vertex>>                     vertices;
        std::vector<std::shared_ptr<Face>>                       faces;
    }; // class HalfEdgeDS END

public:
    explicit Mesh3D(const std::vector<Eigen::Vector3f>& vertexMatrix,
                    const std::vector<uint32_t>&        indexVector);

    bool HasVertices(void) { return !m_vertices.empty(); };
    bool HasUVCoordinates(void) { return !m_uvCoordinates.empty(); };
    bool HasNormals(void) { return GetVertexAttribute(EVertexAttribute::Normal) != nullptr; };

    VertexAttribute& AddVertexAttribute(EVertexAttribute vertexAttribute);

    VertexAttribute*   GetVertexAttribute(EVertexAttribute vertexAttribute);
    TriangleAttribute* GetTriangleAttribute(ETriangleAttribute triangleAttribute);

    friend const std::vector<uint32_t>& vertexIterator(Mesh3D& mesh);

private:
    std::vector<uint32_t>        m_indices;
    std::vector<Eigen::Vector3f> m_vertices;
    std::vector<Eigen::Vector2f> m_uvCoordinates;

    std::vector<std::unique_ptr<VertexAttribute>>   m_vertexAttributes;
    std::vector<std::unique_ptr<TriangleAttribute>> m_TriangleAttributes;


private:
    HalfEdgeDS m_halfEdgeDS;
};

const std::vector<uint32_t>& vertexIterator(Mesh3D& mesh)
{
    return mesh.m_indices;
}

#endif
