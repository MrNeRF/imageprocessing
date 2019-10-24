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
    Eigen::Vector3f GetColor(void) const override { return m_color; };
    void            SetColor(const Eigen::Vector3f& color) override { m_color = color; };
    bool            CheckCollision(const Eigen::Vector2f& pointToTest) const override;

private:
    void createCircle(float radius, Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>& vertices);

private:
    unsigned int VAO, VBO, EBO;
    std::vector<int>                                         m_indices;
    Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor> m_vertices;
    Eigen::Vector2f                                          m_center;

    Eigen::Vector3f m_color = {0.0f, 0.0f, 1.0f};
};
#endif
