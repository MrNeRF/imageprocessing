#include "Line.h"
#include "Constants.h"
#include "CrossingNumberPPolygon.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Line::Line(const Eigen::Vector2f& p1, const Eigen::Vector2f& p2, const float width)
    : m_p1(p1)
    , m_p2(p2)
    , m_width(width)
{
    m_normal    = m_p2 - m_p1;
    float t     = m_normal(0);
    m_normal(0) = -m_normal(1);
    m_normal(1) = t;
    m_normal(0) = -m_p1(1);
    m_normal.normalize();

    createLine();
    dataObject.CreateVertices(vertices, indices);
    dataObject.CreateColor(Color(.0f, .0f, 1.0f));
}

void Line::createLine(void)
{
    // point 1;
    vertices.block(0, 0, 1, 2) = (-1 * m_normal * m_width + m_p1).transpose();
    // point 2
    vertices.block(1, 0, 1, 2) = (m_normal * m_width + m_p1).transpose();
    // point3
    vertices.block(2, 0, 1, 2) = (m_normal * m_width + m_p2).transpose();
    // point4
    vertices.block(3, 0, 1, 2) = (-1 * m_normal * m_width + m_p2).transpose();

    collisionPoints.push_back(-1 * m_normal * m_width + m_p1);
    collisionPoints.push_back(m_normal * m_width + m_p1);
    collisionPoints.push_back(m_normal * m_width + m_p2);
    collisionPoints.push_back(-1 * m_normal * m_width + m_p2);
    indices = {0, 1, 3, 2, 3, 0};
}

void Line::Draw(void) const
{
    dataObject.DrawObject(GL_TRIANGLES);
}

bool Line::CheckCollision(const Eigen::Vector2f& pointToTest) const
{
    CrossingNumberPPolygon collision(collisionPoints, pointToTest);
    collision.Compute();
    return collision.GetOutput();
}
