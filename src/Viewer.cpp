#include "Viewer.h"
#include "Camera.h"
#include "Object3D.h"
#include "Shader.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <cmath>

#define GLM_FORCE_CXX14
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Viewer::Viewer(std::unique_ptr<Window> window)
    : m_window(std::move(window))
{
}

void Viewer::Run(void)
{
    Shader modelShader("3D Model Shader");
    modelShader.InitShaders("../Shaders/modelShader.vs", "../Shaders/modelShader.fs");

    Eigen::Vector3f lightPos   = Eigen::Vector3f(5.f, 5.f, 1.0f);
    Eigen::Vector3f lightColor = Eigen::Vector3f(1.0f, 1.0f, 1.0f);

    modelShader.SetVector("lightPosition", lightPos);
    modelShader.SetVector("lightColor", lightColor);

    Object3D suzanne("../models/suzanne.obj");

    float vertices[] = {
        // positions          // colors            coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // top left
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    Eigen::Vector3f eye{0.f, 0.f, 4.f};

    while (!glfwWindowShouldClose(m_window->GetGLFWWindow()))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_window->GetGLFWWindow(), true);
        }

        if (m_window->keyboardDevice.key == GLFW_KEY_W
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            eye += Eigen::Vector3f(0.0f, 0.f, 0.1f);
            std::cout << "W" << std::endl;
        }
        if (m_window->keyboardDevice.key == GLFW_KEY_S
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            eye += Eigen::Vector3f(0.0f, 0.f, -0.1f);
            std::cout << "S" << std::endl;
        }
        if (m_window->keyboardDevice.key == GLFW_KEY_D
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            eye += Eigen::Vector3f(-0.1f, 0.f, 0.f);
            std::cout << "D" << std::endl;
        }
        if (m_window->keyboardDevice.key == GLFW_KEY_A
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            eye += Eigen::Vector3f(.1f, 0.f, 0.f);
            std::cout << "A" << std::endl;
        }

        modelShader.UseShader();
        Eigen::Vector3f target{0.f, 0.f, 0.f};
        Eigen::Vector3f up{0.f, 1.f, 0.f};
        Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
        model(0, 0)           = 1.5f;
        model(2, 3)           = 2.f;
        Eigen::Matrix4f view  = Camera::LookAt(eye, target, up);
        Eigen::Matrix4f projection = Camera::PerspectiveProjection(45.f, 800.f / 600.f, 0.1f, 50.f);

        /* glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 50.0f); */

        modelShader.SetTransformationMatrix("view", view);
        modelShader.SetTransformationMatrix("projection", projection);
        modelShader.SetTransformationMatrix("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* suzanne.Draw(); */
        glfwSwapBuffers(m_window->GetGLFWWindow());
        glfwPollEvents();
    }
}

