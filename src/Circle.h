#ifndef CIRCLE_H
#define CIRCLE_H

#include "Eigen/Dense"
#include "Primitives2D.h"
#include <vector>

class Circle : Primitives2D
{
public:
    Circle(float radius = 1.0f, int numberOfPoints = 10);
    virtual ~Circle(void) = default;

public:
    void draw(void) override;

private:
    void createCircle(float radius, Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>& vertices);

private:
    unsigned int VAO, VBO, EBO;
    std::vector<int> indices;
    Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor> vertices;
};
#endif
