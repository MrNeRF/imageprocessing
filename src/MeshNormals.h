#ifndef MESH_NORMALS_H
#define MESH_NORMALS_H

#include "Eigen/Dense"
#include <vector>

class MeshNormals
{
public:
    bool Compute(const Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>& vertices,
                 std::vector<int>                                                indices);
};

#endif
