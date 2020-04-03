#ifndef VERTEX_ATTRIBUTE
#define VERTEX_ATTRIBUTE

#include <Eigen/Dense>

struct VertexAttribute
{
	virtual ~VertexAttribute() {};
};

struct VertexColorAttribute : public VertexAttribute
{
    Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor> vertexColor = Eigen::MatrixX2f::Zero(1, 2);
};

struct VertexNormalAttribute : public VertexAttribute
{
    Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> vertices      = Eigen::MatrixX3f::Zero(1, 3);
};
#endif
