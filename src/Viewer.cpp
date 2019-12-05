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

    Eigen::Vector3f lightPos   = Eigen::Vector3f(50.f, 50.f, 10.0f);
    Eigen::Vector3f lightColor = Eigen::Vector3f(0.8f, 0.8f, .8f);

    glEnable(GL_DEPTH_TEST);
    Object3D suzanne("../models/suzanne.obj");

    Eigen::Vector3f eye{0.f, 0.f, 4.f};

    while (!glfwWindowShouldClose(m_window->GetGLFWWindow()))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_window->GetGLFWWindow(), true);
        }

        if (m_window->keyboardDevice.key == GLFW_KEY_W
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            eye += Eigen::Vector3f(0.0f, 0.f, 0.1f);
        }
        if (m_window->keyboardDevice.key == GLFW_KEY_S
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            eye += Eigen::Vector3f(0.0f, 0.f, -0.1f);
        }
        if (m_window->keyboardDevice.key == GLFW_KEY_D
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            eye += Eigen::Vector3f(-0.1f, 0.f, 0.f);
        }
        if (m_window->keyboardDevice.key == GLFW_KEY_A
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            eye += Eigen::Vector3f(.1f, 0.f, 0.f);
        }

        modelShader.UseShader();
        Eigen::Vector3f target{0.f, 0.f, 0.f};
        Eigen::Vector3f up{0.f, 1.f, 0.f};
        Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
        model(0, 0)           = 1.5f;
        model(2, 3)           = 2.f;
        Eigen::Matrix4f view  = Camera::LookAt(eye, target, up);
        Eigen::Matrix4f projection = Camera::PerspectiveProjection(45.f, 800.f / 600.f, 0.1f, 50.f);

        modelShader.SetTransformationMatrix("view", view);
        modelShader.SetTransformationMatrix("projection", projection);
        modelShader.SetTransformationMatrix("model", model);

        modelShader.SetVector("lightPosition", lightPos);
        modelShader.SetVector("lightColor", lightColor);

        suzanne.Draw();
        glfwSwapBuffers(m_window->GetGLFWWindow());
        glfwPollEvents();
    }
}

