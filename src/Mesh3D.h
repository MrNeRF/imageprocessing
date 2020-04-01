#ifndef MESH_3D_H
#define MESH_3D_H

#include "HalfEdgeDS.h"
#include <Eigen/Dense>
#include <vector>

class Mesh3D
{
public:
    Mesh3D(void) = default;
    Mesh3D(Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> vertexMatrix, std::vector<int> indexVector);

    bool HasVertices(void) { return !vertices.isZero(); };
    bool HasUVCoordinates(void) { return !uvCoordinates.isZero(); };
    bool HasNormals(void) { return !normals.isZero(); };
    // Indices for element buffer object (EBO)
    std::vector<int> indices;
    Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> vertices      = Eigen::MatrixX3f::Zero(3, 3);
    Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor> uvCoordinates = Eigen::MatrixX2f::Zero(2, 2);
    Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> normals       = Eigen::MatrixX3f::Zero(3, 3);

    void initializeMeshDS();

private:
    HalfEdgeDS meshDS;
};

#endif
