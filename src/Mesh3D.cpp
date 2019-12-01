#include "Mesh3D.h"

Mesh3D::Mesh3D(Eigen::MatrixX3f vertexMatrix, std::vector<int> indexVector)
    : indices{indexVector}
    , vertices(vertexMatrix)
{
}
