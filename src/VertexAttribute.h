#ifndef VERTEX_ATTRIBUTE
#define VERTEX_ATTRIBUTE

#include <Eigen/Dense>

struct VertexAttribute
{
	virtual ~VertexAttribute() {};
};

struct VertexColorAttribute : public VertexAttribute
{
    VertexColorAttribute(void) = default;
    VertexColorAttribute(Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>& vertexColorAttribute)
        : m_vertexColor{vertexColorAttribute} {};
    Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor> m_vertexColor = Eigen::MatrixX2f::Zero(1, 2);
};

struct VertexNormalAttribute : public VertexAttribute
{
    VertexNormalAttribute(void) = default;
    VertexNormalAttribute(Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>& vertexNormalAttribute)
        : m_vertexColor{vertexNormalAttribute} {};
    Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> m_vertexColor = Eigen::MatrixX3f::Zero(1, 3);
};
#endif
