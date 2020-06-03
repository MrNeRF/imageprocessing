#ifndef TRIANGLE_ATTRIBUTE
#define TRIANGLE_ATTRIBUTE

#include <Eigen/Dense>

struct TriangleAttribute
{
public:
    virtual ~TriangleAttribute(){};
};

struct TriangleNormalAttribute : public TriangleAttribute
{
public:
    TriangleNormalAttribute();

    Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> triangleNormals = Eigen::MatrixX3f::Zero(1, 3);
};
#endif
