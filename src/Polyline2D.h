#ifndef POLYLINE_H
#define POLYLINE_H

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
    void Draw(void) const override;
    Eigen::Vector3f GetColor(void) const override { return m_color; };
    void            SetColor(const Eigen::Vector3f& color) override;
    bool            CheckCollision(const Eigen::Vector2f& pointToTest) const override;
    void AddLines(std::unique_ptr<Line> line);

private:
    std::vector<std::unique_ptr<Primitives2D>> m_polyline;
    Eigen::Vector3f                            m_color = {0.0f, 0.0f, 1.0f};
};
#endif
