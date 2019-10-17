#ifndef CIRCLE_H
#define CIRCLE_H

#include "Eigen/Dense"
#include "Primitives2D.h"
#include <vector>

class Circle : public Primitives2D
{
public:
    Circle(Eigen::Vector2f center, const float radius, const int numberOfPoints = 10);
    virtual ~Circle(void) = default;

public:
    void Draw(void) const override;

private:
    void createCircle(float radius, Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>& vertices);

private:
    unsigned int VAO, VBO, EBO;
    std::vector<int>                                         m_indices;
    Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor> m_vertices;
    Eigen::Vector2f                                          m_center;
};
#endif
