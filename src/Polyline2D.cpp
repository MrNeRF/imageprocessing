#include "Polyline2D.h"
#include "Circle.h"

void Polyline2D::AddLines(std::unique_ptr<Line> line)
{
    Eigen::Vector2f p1, p2;
    line->GetEndPoints(p1, p2);
    line->SetColor(color);
    float radius = 0.01f;

    if (m_polyline.size() == 0)
    {
        m_polyline.push_back(std::make_unique<Circle>(p1, radius, 20));
        m_polyline.push_back(std::move(line));
        m_polyline.push_back(std::make_unique<Circle>(p2, radius, 20));
    }
    else
    {
        m_polyline.push_back(std::move(line));
        m_polyline.push_back(std::make_unique<Circle>(p2, radius, 20));
    }
}

void Polyline2D::Draw(void) const
{
    for (const auto& p : m_polyline)
    {
        p->Draw();
    }
}

bool Polyline2D::CheckCollision(const Eigen::Vector2f& pointToTest) const
{
    for (const auto& p : m_polyline)
    {
        if (p->CheckCollision(pointToTest))
        {
            p->SetColor({1.f, 1.f, 0.4f});
            return true;
        }
    }
    return false;
}

