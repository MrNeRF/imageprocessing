#ifndef POLYLINE_H
#define POLYLINE_H

#include "Color.h"
#include "Eigen/Dense"
#include "Line.h"
#include "Primitives2D.h"
#include <memory>
#include <vector>

class Polyline2D final : public Primitives2D
{
public:
    Polyline2D(void)          = default;
    virtual ~Polyline2D(void) = default;

public:
    void  Draw(void) const override;
    Color GetColor(void) const override { return color; };
    void  SetColor(const Color& rgb) override { color = rgb; };
    bool  CheckCollision(const Eigen::Vector2f& pointToTest) const override;
    void  AddLines(std::unique_ptr<Line> line);

private:
    std::vector<std::unique_ptr<Primitives2D>> m_polyline;
    Color                                      color;
};
#endif
