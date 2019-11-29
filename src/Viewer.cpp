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

    Image image1("../images/fibi.jpg");
    imageShader.UseShader(); // don't forget to activate/use the shader before setting uniforms!
    imageShader.SetValue("texture1", 0);

    Polyline2D polyline;
    polyline.AddLines(std::make_unique<Line>(Line({0.0f, 0.0f}, {0.4f, 0.4f}, 0.005)));
    polyline.AddLines(std::make_unique<Line>(Line({0.4f, 0.4f}, {0.4f, 0.5f}, 0.005)));
    polyline.AddLines(std::make_unique<Line>(Line({0.4f, 0.5f}, {0.5f, 0.0f}, 0.005)));
    polyline.AddLines(std::make_unique<Line>(Line({0.5f, 0.0f}, {0.0f, -0.2f}, 0.005)));

    Rectangle canvas(Eigen::Vector2f(-0.5f, -0.5f), 1.0f, 1.0f);

    while (!glfwWindowShouldClose(m_window->GetGLFWWindow()))
    {
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

        pointShader.UseShader();
        /* canvas.Draw(); */
        polyline.Draw();
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window->GetGLFWWindow());
        glfwPollEvents();
    }
}

