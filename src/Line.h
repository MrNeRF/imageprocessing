#ifndef LINE_H
#define LINE_H

#include "Color.h"
#include "Eigen/Dense"
#include "Primitives2D.h"
#include <vector>

class Line : public Primitives2D
{
public:
    Line(const Eigen::Vector2f& p1, const Eigen::Vector2f& p2, const float width);
    virtual ~Line(void) = default;

public:
    void  Draw(void) const override;
    Color GetColor(void) const override { return color; };
    void  SetColor(const Color& rgb) override { color = rgb; };
    bool  CheckCollision(const Eigen::Vector2f& pointToTest) const override;
    void  SetPosition(Eigen::Vector2f& p1, Eigen::Vector2f& p2);
    void  SetWidth(float width);
    void  GetEndPoints(Eigen::Vector2f& p1, Eigen::Vector2f& p2)
    {
        p1 = m_p1;
        p2 = m_p2;
    };
    float GetWidth(void) { return m_width; };
    void  GetVertices(std::vector<Eigen::Vector2f>& v) { v = collisionPoints; };

private:
    void createLine(void);

private:
    unsigned int    VAO, VBO, EBO;
    Eigen::Vector2f m_p1, m_p2;
    Eigen::Vector2f m_normal;
    float           m_width = 0.01f;

    std::vector<Eigen::Vector2f>                collisionPoints;
    std::vector<int>                            indices;
    Eigen::Matrix<float, 4, 2, Eigen::RowMajor> vertices;

    Color color;
};
#endif
