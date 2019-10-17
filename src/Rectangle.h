#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Eigen/Dense"
#include "Primitives2D.h"
#include <vector>

class Rectangle : public Primitives2D
{
public:
    Rectangle(const Eigen::Vector2f& p1, const float width, const float height);
    virtual ~Rectangle(void) = default;

public:
    void Draw(void) const override;
    void SetPosition(Eigen::Vector2f& p1);
    void SetWidth(float width, float height);

private:
    void createRectangle(void);

private:
    unsigned int VAO, VBO, EBO;
    Eigen::Vector2f m_p1, m_p2;

    float m_width  = 0.01f;
    float m_height = 0.01f;

    std::vector<int> indices;
    Eigen::Matrix<float, 4, 2, Eigen::RowMajor> vertices;
};
#endif
