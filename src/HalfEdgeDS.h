#ifndef HALF_EDGE_DS_H
#define HALF_EDGE_DS_H

#include "Mesh3D.h"
#include <Eigen/Dense>
#include <map>
#include <memory>
#include <utility>
#include <vector>

class HalfEdgeDS
{
public:
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
    void Initialize(Mesh3D* meshData);

private:
    void fillDataByFace(const std::pair<int, int>& e0,
                        const std::pair<int, int>& e1,
                        const std::pair<int, int>& e2,
                        std::shared_ptr<Vertex>    vertex0,
                        std::shared_ptr<Vertex>    vertex1,
                        std::shared_ptr<Vertex>    vertex2);

private:
    std::map<std::pair<int, int>, std::shared_ptr<HalfEdge>> edges;
    std::vector<std::shared_ptr<Vertex>>                     vertices;
    std::vector<std::shared_ptr<Face>>                       faces;
};

#endif
