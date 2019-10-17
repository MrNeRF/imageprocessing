#include "Line.h"
#include "Constants.h"
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    // unbind
    glBindVertexArray(0);
}

void Line::createLine(void)
{
    // point 1;
    vertices.block(0, 0, 1, 2) = (m_normal * m_width + m_p1).transpose();
    // point 2
    vertices.block(1, 0, 1, 2) = (-1 * m_normal * m_width + m_p1).transpose();

    // point3
    vertices.block(2, 0, 1, 2) = (m_normal * m_width + m_p2).transpose();
    // point4
    vertices.block(3, 0, 1, 2) = (-1 * m_normal * m_width + m_p2).transpose();

    std::cout << vertices(0, 0) << ", " << vertices(0, 1) << "\n";
    std::cout << vertices(1, 0) << ", " << vertices(1, 1) << "\n";
    std::cout << vertices(2, 0) << ", " << vertices(2, 1) << "\n";
    std::cout << vertices(3, 0) << ", " << vertices(3, 1) << "\n";
    indices = {0, 1, 3, 2, 3, 0};
}

void Line::Draw(void) const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
