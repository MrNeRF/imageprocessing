#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Color.h"
#include "Eigen/Dense"
#include "OpenGL2DDataObject.h"
#include "Primitives2D.h"
#include <memory>
#include <vector>

class TextureObject;

class Rectangle : public Primitives2D
{
public:
    Rectangle(const Eigen::Vector2f& p1, const float width, const float height);
    virtual ~Rectangle(void) = default;

public:
    void  Draw(void) const override;
    Color GetColor(void) const override { return color; };
    void  SetColor(const Color& rgb) override { color = rgb; };
    bool  CheckCollision(const Eigen::Vector2f& pointToTest) const override;
    void  SetPosition(Eigen::Vector2f& p1);
    void  SetWidth(float width, float height);

private:
    void createRectangle(void);

private:
    Eigen::Vector2f m_p1, m_p2;

    float m_width  = 0.01f;
    float m_height = 0.01f;

    Color                                       color;
    OpenGL2DDataObject                          dataObject;
    std::vector<Eigen::Vector2f>                collisionPoints;
    std::vector<int>                            indices;
    Eigen::Matrix<float, 4, 2, Eigen::RowMajor> vertices;
    std::unique_ptr<TextureObject>              spTextureObj;
};

#endif
