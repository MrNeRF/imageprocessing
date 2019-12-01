#ifndef MESH_3D_H
#define MESH_3D_H

#include <Eigen/Dense>
#include <vector>

class Mesh3D
{
public:
    Mesh3D(void) = default;
    Mesh3D(Eigen::MatrixX3f vertexMatrix, std::vector<int> indexVector);
    // @TODO : Weitere Attribute einbauen.

    // Wegen Obj Formats crashed es momentan noch
    Eigen::MatrixX3f vertices = Eigen::MatrixX3f::Zero(3, 3);
    std::vector<int> indices;
};

#endif
