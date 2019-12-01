#ifndef MESH_3D_H
#define MESH_3D_H

#include <Eigen/Dense>
#include <vector>

class Mesh3D
{
public:
    Mesh3D(void) = default;
    Mesh3D(Eigen::MatrixX3f vertexMatrix, std::vector<int> indexVector);

    bool HasVertices(void) { return vertices != Eigen::MatrixX3f::Zero(3, 3); };
    bool HasUVCoordinates(void) { return uvCoordinates != Eigen::MatrixX2f::Zero(2, 2); };
    bool HasNormals(void) { return normals != Eigen::MatrixX3f::Zero(3, 3); };
    // Indices for element buffer object (EBO)
    std::vector<int> indices;
    Eigen::MatrixX3f vertices      = Eigen::MatrixX3f::Zero(3, 3);
    Eigen::MatrixX2f uvCoordinates = Eigen::MatrixX2f::Zero(2, 2);
    Eigen::MatrixX3f normals       = Eigen::MatrixX3f::Zero(3, 3);
};

#endif
