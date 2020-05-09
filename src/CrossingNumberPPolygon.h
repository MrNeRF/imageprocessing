#ifndef CROSSING_NUMBER_PPOLYGON_H
#define CROSSING_NUMBER_PPOLYGON_H

#include "Algorithm.h"
#include "Eigen/Dense"
#include <vector>

class CrossingNumberPPolygon : public Algorithm
{
public:
    explicit CrossingNumberPPolygon(const std::vector<Eigen::Vector2f>& vertices, const Eigen::Vector2f point)
        : m_vertices{vertices}
        , m_point{point} {};

    ~CrossingNumberPPolygon(void) = default;

    bool Compute(void) override;
    bool GetOutput(void) noexcept { return m_result; };

private:
    std::vector<Eigen::Vector2f>       m_vertices;
    Eigen::Vector2f                    m_point;
    bool                               m_result = false;
};
#endif

