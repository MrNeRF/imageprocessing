#ifndef TRIANGLE_NORMAL_ATTRIBUTE
#define TRIANGLE_NORMAL_ATTRIBUTE

#include "TriangleAttribute.h"
#include <Eigen/Dense>

class TriangleNormalAttribute : public TriangleAttribute
{
	public:
		TriangleNormalAttribute();

	private:
		Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> triangleNormals = Eigen::MatrixX3f::Zero(1, 3);
};
#endif
