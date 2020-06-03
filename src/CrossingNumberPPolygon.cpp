#include "CrossingNumberPPolygon.h"

bool CrossingNumberPPolygon::Compute(void)
{
    uint32_t len = m_vertices.size();
    m_vertices.push_back(m_vertices[0]);
    uint32_t crossingNumberCounter = 0;

    for (uint32_t i = 0; i < len; ++i)
    {
        if ((m_vertices[i].y() <= m_point.y()
             && m_vertices[i + 1].y() >= m_point.y())
            || (m_vertices[i].y() > m_point.y() && m_vertices[i + 1].y() <= m_point.y()))
        {
            float vt = (m_point.y() - m_vertices[i].y())
                       / (m_vertices[i + 1].y() - m_vertices[i].y());

            if (m_point.x() < m_vertices[i].x() + vt * (m_vertices[i + 1]).x() - m_vertices[i].x())
            {
                ++crossingNumberCounter;
            }
        }
    }

    m_result = crossingNumberCounter % 2 == 0 ? false : true;
    return true;
}
