#include "Viewer.h"
#include "Camera.h"
#include "Object3D.h"
#include "Shader.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <cmath>

#ifndef M_PI
#define M_PI (4.0 * std::atan2(1.0, 1.0))
#endif

template<typename N>
inline N deg2rad(N d)
{
    return M_PI * d / 180.0;
}

Viewer::Viewer(std::unique_ptr<Window> window)
    : m_window(std::move(window))
{
}

void Viewer::Run(void)
{
    Shader modelShader("3D Model Shader");
    modelShader.InitShaders("../Shaders/modelShader.vs", "../Shaders/modelShader.fs");
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    modelShader.SetTransformationMatrix("model", model);

    Eigen::Vector3f eye{0.f, 0.f, 3.f};
    Eigen::Vector3f target{0.f, 0.f, 0.f};
    Eigen::Vector3f up{0.f, 1.f, 0.f};
    Eigen::Matrix4f viewMatrix = Camera::LookAt(eye, target, up);
    Eigen::Matrix4f pProjMatrix = Camera::PerspectiveProjection(deg2rad(45.f), m_window->aspectRatio, 0.1f, 100.f);

    modelShader.SetTransformationMatrix("view", viewMatrix);
    modelShader.SetTransformationMatrix("projection", pProjMatrix);

    Eigen::Vector3f lightPos   = Eigen::Vector3f(30.f, 15.f, 1.0f);
    Eigen::Vector3f lightColor = Eigen::Vector3f(0.9f, 0.9f, 0.9f);

    modelShader.SetVector("lightPosition", lightPos);
    modelShader.SetVector("lightColor", lightColor);

    Object3D suzanne("../models/suzanne.obj");

    while (!glfwWindowShouldClose(m_window->GetGLFWWindow()))
    {
        if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_window->GetGLFWWindow(), true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        modelShader.UseShader();
        suzanne.Draw();
        glfwSwapBuffers(m_window->GetGLFWWindow());
        glfwPollEvents();
    }
}

