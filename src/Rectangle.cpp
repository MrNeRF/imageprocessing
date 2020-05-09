#include "Rectangle.h"
#include "Constants.h"
#include "CrossingNumberPPolygon.h"
#include "TextureObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Rectangle::Rectangle(const Eigen::Vector2f& p1, const float width, const float height)
    : m_p1(p1)
    , m_width(width)
    , m_height(height)
{
    createRectangle();
    dataObject.CreateVertices(vertices, indices);
    dataObject.CreateTextureCoordinates(vertices);
}

void Rectangle::createRectangle(void)
{
    // point 1;
    Eigen::Vector2f xDir{1.0f, 0.0f};
    Eigen::Vector2f yDir{0.0f, 1.0f};
    vertices.block(0, 0, 1, 2) = (m_p1).transpose();
    // point 2
    vertices.block(1, 0, 1, 2) = (m_p1 + m_width * xDir).transpose();
    // point3
    vertices.block(2, 0, 1, 2) = (m_p1 + m_width * xDir + m_height * yDir).transpose();
    // point4
    vertices.block(3, 0, 1, 2) = (m_p1 + yDir * m_height).transpose();

    collisionPoints.emplace_back(m_p1);
    collisionPoints.emplace_back(m_p1 + m_width * xDir);
    collisionPoints.emplace_back(m_p1 + m_width * xDir + m_height * yDir);
    collisionPoints.emplace_back(m_p1 + yDir * m_height);

    indices = {0, 1, 2,
               2, 3, 0};
}

void Rectangle::Draw(void) const
{
    dataObject.DrawObject(GL_TRIANGLES);
}

bool Rectangle::CheckCollision(const Eigen::Vector2f& pointToTest) const
{
    CrossingNumberPPolygon collision(collisionPoints, pointToTest);
    collision.Compute();
    return collision.GetOutput();
}
