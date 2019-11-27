#include "Viewer.h"
#include "Image.h"
#include "Line.h"
#include "Polyline2D.h"
#include "Rectangle.h"
#include "Shader.h"
#include "Window.h"
#include <GLFW/glfw3.h>

Viewer::Viewer(std::unique_ptr<Window> window)
    : m_window(std::move(window))
{
}

Viewer::~Viewer(void)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Viewer::Run(void)
{
    Shader imageShader("ImageShader");
    imageShader.InitShaders("../Shaders/image.vs", "../Shaders/image.fs");
    Shader pointShader("PointShader");
    pointShader.InitShaders("../Shaders/points.vs", "../Shaders/points.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
        -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
        0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f // bottom left
    };

    unsigned int indices[] = {
        0, 1, 2, // first triangle
        2, 3, 0  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Image image1("../images/fibi.jpg");

    Polyline2D polyline;
    polyline.AddLines(std::make_unique<Line>(Line({0.0f, 0.0f}, {0.4f, 0.4f}, 0.005)));
    polyline.AddLines(std::make_unique<Line>(Line({0.4f, 0.4f}, {0.4f, 0.5f}, 0.005)));
    polyline.AddLines(std::make_unique<Line>(Line({0.4f, 0.5f}, {0.5f, 0.0f}, 0.005)));
    polyline.AddLines(std::make_unique<Line>(Line({0.5f, 0.0f}, {0.0f, -0.2f}, 0.005)));

    Eigen::Vector2f p3(-0.2, 0.2);
    Rectangle       rectangle(p3, 0.2f, 0.25f);
    // -------------------------------------------------------------------------------------------
    imageShader.UseShader(); // don't forget to activate/use the shader before setting uniforms!
    imageShader.SetValue("texture1", 0);

    while (!glfwWindowShouldClose(m_window->GetGLFWWindow()))
    {
        // input
        // -----
        if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_window->GetGLFWWindow(), true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        image1.Use();
        // render container
        imageShader.UseShader();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        pointShader.UseShader();
        pointShader.SetVector("CustomColor", Eigen::Vector3f(0.0f, 1.0f, 0.0f));
        polyline.Draw();
        pointShader.SetVector("CustomColor", Eigen::Vector3f(0.0f, 0.0f, 1.0f));
        rectangle.Draw();
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window->GetGLFWWindow());
        glfwPollEvents();
    }
}

