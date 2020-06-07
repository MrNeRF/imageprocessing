#ifndef MESH_3D_H
#define MESH_3D_H

#include "Defs.h"
#include "FaceAttribute.h"
#include "Logger.h"
#include "VertexAttribute.h"
#include <Eigen/Dense>
#include <map>
#include <memory>
#include <string>
#include <tuple>
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

    enum class EFaceAttribute
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
            Vertex(uint32_t vertexID, const Eigen::Vector3f& pos)
                : id{vertexID}
                , position{pos} {};

            uint32_t                id;
            const Eigen::Vector3f&  position;
            std::weak_ptr<HalfEdge> wspOutgoingHalfEdge;
        };

        struct Face
        {
            Face(uint32_t faceID)
                : id{faceID} {};
            uint32_t                id;
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
        friend class Mesh3D;
        friend class oneRingFaceIterator;
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
                    const std::vector<uint32_t>&        indexVector,
                    const std::string&                  name);

    bool                                                          HasVertices(void) { return !m_vertices.empty(); };
    bool                                                          HasUVCoordinates(void) { return !m_uvCoordinates.empty(); };
    bool                                                          HasNormals(void) { return GetVertexAttribute(EVertexAttribute::Normal) != nullptr; };
    std::tuple<Eigen::Vector3f, Eigen::Vector3f, Eigen::Vector3f> GetFaceVertices(uint32_t faceIndex);
    VertexAttribute&                                              AddVertexAttribute(EVertexAttribute vertexAttribute);
    FaceAttribute&                                                AddFaceAttribute(EFaceAttribute faceAttribute);

    const std::vector<Eigen::Vector3f>& GetVertices() const { return m_vertices; }
    const std::vector<uint32_t>&        GetIndices() const { return m_indices; }
    uint32_t                            GetNumberOfVertice() const { return m_vertices.size(); }
    uint32_t                            GetNumberOfFaces() const { return static_cast<uint32_t>(static_cast<float>(m_indices.size()) / 3.f); }
    const Eigen::Vector3f&              GetVertex(uint32_t index) const
    {
        ASSERT(index < m_vertices.size());
        return m_vertices[index];
    }
    VertexAttribute* GetVertexAttribute(EVertexAttribute vertexAttribute);
    FaceAttribute*   GetFaceAttribute(EFaceAttribute triangleAttribute);

    void IterateAllFaces() const;

private:
    friend class OpenGL3DDataObject;
    friend class vertexIterator;
    friend class faceIterator;
    friend class oneRingFaceIterator;
    std::string                  m_name;
    std::vector<uint32_t>        m_indices;
    std::vector<Eigen::Vector3f> m_vertices;
    std::vector<Eigen::Vector2f> m_uvCoordinates;

    std::vector<std::unique_ptr<VertexAttribute>> m_vertexAttributes;
    std::vector<std::unique_ptr<FaceAttribute>>   m_FaceAttributes;
    HalfEdgeDS                                    m_halfEdgeDS;
};

class vertexIterator
{
public:
    explicit vertexIterator(const Mesh3D& rMesh)
        : m_rMesh(rMesh)
    {
    }

    uint32_t        operator*() { return idx; }
    vertexIterator& operator++()
    {
        ++idx;
        return *this;
    }
    bool operator!=(const vertexIterator& other) { return idx != other.idx; }

    vertexIterator begin() { return vertexIterator(0, m_rMesh); };
    vertexIterator end() { return vertexIterator(endIdx, m_rMesh); };

private:
    explicit vertexIterator(uint32_t i, const Mesh3D& rMesh)
        : m_rMesh(rMesh)
        , idx{i} {};

private:
    const Mesh3D& m_rMesh;
    uint32_t      idx    = 0;
    uint32_t      endIdx = m_rMesh.m_vertices.size();
};

class faceIterator
{
public:
    explicit faceIterator(Mesh3D& rMesh)
        : m_rMesh{rMesh}
        , endIdx{static_cast<uint32_t>(static_cast<float>(rMesh.m_indices.size()) / 3.f)}
    {
    }

    uint32_t      operator*() { return idx; }
    faceIterator& operator++()
    {
        ++idx;
        return *this;
    }
    bool operator!=(const faceIterator& other) { return idx != other.idx; }

    faceIterator begin() { return faceIterator(m_rMesh, idx, endIdx); };
    faceIterator end() { return faceIterator(m_rMesh, endIdx, endIdx); };

private:
    explicit faceIterator(Mesh3D& rMesh, uint32_t start, uint32_t end)
        : m_rMesh(rMesh)
        , idx{start}
        , endIdx{end}
    {
    }

private:
    Mesh3D&  m_rMesh;
    uint32_t idx    = 0;
    uint32_t endIdx = 0;
};

class oneRingFaceIterator
{
public:
    explicit oneRingFaceIterator(Mesh3D& rMesh, uint32_t vertexID)
        : m_rMesh(rMesh)
        , m_startVertexID(vertexID)
    {
        ASSERT(vertexID >= 0 && vertexID < rMesh.m_vertices.size());
        if (m_rMesh.m_halfEdgeDS.vertices[vertexID] == nullptr)
        {
            m_bStopIterating = true;
        }
        else
        {
            m_wspHalfEdge   = m_rMesh.m_halfEdgeDS.vertices[vertexID]->wspOutgoingHalfEdge;
            m_currentFaceID = m_wspHalfEdge.lock()->spFace->id;
            m_startFaceID   = m_currentFaceID;
        }
    }

    uint32_t             operator*() { return m_currentFaceID; }
    oneRingFaceIterator& operator++()
    {
        m_wspHalfEdge = m_wspHalfEdge.lock()->wspNextHalfeEdge.lock()->wspNextHalfeEdge;
        m_wspHalfEdge = m_wspHalfEdge.lock()->wspOppositeHalfeEdge;
        if (!m_wspHalfEdge.lock())
        {
            m_currentFaceID = m_startFaceID;
            return *this;
        }
        m_currentFaceID = m_wspHalfEdge.lock()->spFace->id;
        return *this;
    }
    bool operator!=(const oneRingFaceIterator& other)
    {
        if (m_bStopIterating)
        {
            m_bStopIterating = false;
            return false;
        }

        auto wspTmpHalfEdge = m_wspHalfEdge.lock()->wspNextHalfeEdge.lock()->wspNextHalfeEdge;
        wspTmpHalfEdge      = wspTmpHalfEdge.lock()->wspOppositeHalfeEdge;
        if (!wspTmpHalfEdge.lock() || wspTmpHalfEdge.lock()->spFace->id == other.m_startFaceID)
        {
            m_bStopIterating = true;
        }
        return true;
    }

    oneRingFaceIterator begin() { return oneRingFaceIterator(m_rMesh, m_startVertexID); };
    oneRingFaceIterator end() { return oneRingFaceIterator(m_rMesh, m_startVertexID); };

private:
    Mesh3D&                                     m_rMesh;
    std::weak_ptr<Mesh3D::HalfEdgeDS::HalfEdge> m_wspHalfEdge;
    uint32_t                                    m_startVertexID  = 0;
    uint32_t                                    m_startFaceID    = 0;
    uint32_t                                    m_currentFaceID  = 0;
    bool                                        m_bStopIterating = false;
};
#endif
