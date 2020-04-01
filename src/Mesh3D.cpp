#include "Mesh3D.h"

Mesh3D::Mesh3D(Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> vertexMatrix, std::vector<int> indexVector)
    : indices{indexVector}
    , vertices(vertexMatrix)
{
}

void Mesh3D::initializeMeshDS()
{
    meshDS.Initialize(vertices, indices);
}
