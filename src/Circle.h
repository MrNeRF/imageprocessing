#ifndef CIRCLE_H
#define CIRCLE_H

#include "Color.h"
#include "Eigen/Dense"
#include "OpenGL2DDataObject.h"
#include "Primitives2D.h"
#include <vector>

using CircleDataType = Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>;

class Circle : public Primitives2D
{
public:
    Circle(Eigen::Vector2f center, const float radius, const int numberOfPoints = 10);
    virtual ~Circle(void) = default;

public:
    void  Draw(void) const override;
    Color GetColor(void) const override { return color; };
    void  SetColor(const Color& rgb) override { color = rgb; };
    bool  CheckCollision(const Eigen::Vector2f& pointToTest) const override;

private:
    void createCircle(float radius, Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>& vertices);

private:
    std::vector<int>   m_indices;
    CircleDataType     m_vertices;
    Eigen::Vector2f    m_center;
    OpenGL2DDataObject dataObject = OpenGL2DDataObject();
    Color              color;
};
#endif
