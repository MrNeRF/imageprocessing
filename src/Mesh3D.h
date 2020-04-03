#ifndef MESH_3D_H
#define MESH_3D_H

#include "TriangleAttribute.h"
#include "VertexAttribute.h"
#include <Eigen/Dense>
#include <map>
#include <memory>
#include <utility>
#include <vector>

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
    explicit Mesh3D(const Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>& vertexMatrix,
                    const std::vector<uint32_t>&                                    indexVector);

    bool HasVertices(void) { return !vertices.isZero(); };
    bool HasUVCoordinates(void) { return !uvCoordinates.isZero(); };
    bool HasNormals(void) { return GetVertexAttribute(EVertexAttribute::Normal) != nullptr; };

    void AddVertexAttribute(std::unique_ptr<VertexAttribute> spVertexAttribute);

    const Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>& GetVertices() { return vertices; }
    const std::vector<uint32_t>&                                    GetIndices() { return indices; };

    VertexAttribute*   GetVertexAttribute(EVertexAttribute vertexAttribute);
    TriangleAttribute* GetTriangleAttribute(ETriangleAttribute triangleAttribute);

private:
    std::vector<uint32_t>                           indices;
    std::vector<std::unique_ptr<VertexAttribute>>   m_vertexAttributes;
    std::vector<std::unique_ptr<TriangleAttribute>> m_TriangleAttributes;

    Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> vertices      = Eigen::MatrixX3f::Zero(1, 3);
    Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor> uvCoordinates = Eigen::MatrixX2f::Zero(1, 2);

private:
    HalfEdgeDS m_halfEdgeDS;
};

#endif
