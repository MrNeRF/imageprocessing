#include "Circle.h"
#include "Constants.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Circle::Circle(const Eigen::Vector2f center, const float radius, const int numberOfPoints)
    : m_vertices{numberOfPoints, 2}
    , m_center{center}
{
    createCircle(radius, m_vertices);
    dataObject.CreateVertices(m_vertices, m_indices);
    std::vector<int> colorIndices(m_indices.size(), 0);
    Color            newColor(0.15f, 1.f, 0.5f);
    dataObject.CreateColor(newColor);
}

void Circle::createCircle(float radius, Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>& vertices)
{
    vertices(0, 0) = m_center(0);
    vertices(0, 1) = m_center(1);

    m_indices.push_back(0);

    float PI2      = 2 * M_PI;
    int   numberOfPoints = vertices.rows() - 1;
    float fraction       = 1.0f / static_cast<float>(numberOfPoints);
    for (int i = 0; i < numberOfPoints; ++i)
    {
        vertices(i + 1, 0) = m_center(0) + radius * std::cos(PI2 * i * fraction) * ASPECTRATIO;
        vertices(i + 1, 1) = m_center(1) + radius * std::sin(PI2 * i * fraction);
        // indices
        m_indices.push_back(i + 1);
    }

    m_indices.push_back(1);
}

void Circle::Draw(void) const
{
    dataObject.DrawObject(GL_TRIANGLE_FAN);
}

bool Circle::CheckCollision(const Eigen::Vector2f& pointToTest) const
{
    /* Collision muss noch implementiert werden */
    return false;
}
