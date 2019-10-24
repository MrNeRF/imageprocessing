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
    Eigen::Vector3f GetColor(void) const override { return m_color; };
    void            SetColor(const Eigen::Vector3f& color) override { m_color = color; };
    bool            CheckCollision(const Eigen::Vector2f& pointToTest) const override;
    void SetPosition(Eigen::Vector2f& p1);
    void SetWidth(float width, float height);

private:
    void createRectangle(void);

private:
    unsigned int VAO, VBO, EBO;
    Eigen::Vector2f m_p1, m_p2;

    float m_width  = 0.01f;
    float m_height = 0.01f;

    std::vector<Eigen::Vector2f>                collisionPoints;
    std::vector<int> indices;
    Eigen::Matrix<float, 4, 2, Eigen::RowMajor> vertices;
    Eigen::Vector3f                             m_color = {1.0f, 0.0f, 1.0f};
};
#endif
