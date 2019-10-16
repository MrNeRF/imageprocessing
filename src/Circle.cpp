#include "Circle.h"
#include "Constants.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Circle::Circle(float radius, int numberOfPoints)
    : vertices{numberOfPoints, 2}
{
    createCircle(radius, vertices);

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

void Circle::createCircle(float radius, Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::RowMajor>& vertices)
{
    vertices(0, 0) = 0.f;
    vertices(0, 1) = 0.f;

    indices.push_back(0);

    float PI2      = 2 * M_PI;
    int   numberOfPoints = vertices.rows() - 1;
    float fraction       = 1.0f / static_cast<float>(numberOfPoints);
    for (int i = 0; i < numberOfPoints; ++i)
    {
        vertices(i + 1, 0) = radius * std::cos(PI2 * i * fraction) * ASPECTRATIO;
        vertices(i + 1, 1) = radius * std::sin(PI2 * i * fraction);
        // indices
        indices.push_back(i + 1);
    }

    indices.push_back(1);
}

void Circle::Draw(void)
{
    glBindVertexArray(VAO);
    glPointSize(10.f);
    glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
