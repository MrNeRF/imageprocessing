#include "Viewer.h"
#include "Camera.h"
#include "Color.h"
#include "Macros.h"
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
    Shader lightShader("Light Cube");
    lightShader.InitShaders("../Shaders/lightShader.vs", "../Shaders/lightShader.fs");

    lightShader.UseShader();
    Eigen::Vector3f lightPos = Eigen::Vector3f(10.f, 10.f, 0.0f);
    Color           lightColor(1.f, 1.f, 1.f);
    lightShader.SetVector("lightPosition", lightPos);

    glEnable(GL_DEPTH_TEST);
    Object3D suzanne("../models/suzanne.obj");
    Object3D lightCube("../models/quader.obj");
    lightCube.SetColor(lightColor);

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
            suzanne.UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(5.f), Eigen::Vector3f::UnitX()));
        }
        if (m_window->keyboardDevice.key == GLFW_KEY_S
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            suzanne.UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(-5.f), Eigen::Vector3f::UnitX()));
        }
        if (m_window->keyboardDevice.key == GLFW_KEY_D
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            suzanne.UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(5.f), Eigen::Vector3f::UnitY()));
        }
        if (m_window->keyboardDevice.key == GLFW_KEY_A
            && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS)
        {
            suzanne.UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(-5.f), Eigen::Vector3f::UnitY()));
        }

        modelShader.UseShader();
        Eigen::Vector3f target{0.f, 0.f, 0.f};
        Eigen::Vector3f up{0.f, 1.f, 0.f};
        Eigen::Matrix4f view  = Camera::LookAt(eye, target, up);
        Eigen::Matrix4f projection = Camera::PerspectiveProjection(45.f, 800.f / 600.f, 0.1f, 50.f);

		suzanne.SetPosition({0.f,0.f, 0.f, 10.f});
        modelShader.SetVector("transform.position", suzanne.GetPosition());
        modelShader.SetQuat("transform.qOrientation", suzanne.GetOrientation());
        modelShader.SetQuat("transform.qconjOrientation", suzanne.GetOrientation().conjugate());

        modelShader.SetTransformationMatrix("cameraPos", eye);
        modelShader.SetTransformationMatrix("view", view);
        modelShader.SetTransformationMatrix("projection", projection);

        modelShader.SetVector("light.ambient", Eigen::Vector3f(.5f, .5f, .5f));
        modelShader.SetVector("light.diffuse", Eigen::Vector3f(.2f, .2f, .2f));
        modelShader.SetVector("light.specular", Eigen::Vector3f(1.f, 1.f, 1.f));

        modelShader.SetVector("material.ambient", Eigen::Vector3f(.2125f, 0.1275f, 0.054f));
        modelShader.SetVector("material.diffuse", Eigen::Vector3f(.714f, 0.4284f, 0.18144f));
        modelShader.SetVector("material.specular", Eigen::Vector3f(.393548f, 0.271906f, 0.166721f));
        modelShader.SetValue("material.shininess", .2f);

        modelShader.SetVector("lightPosition", lightPos);
        modelShader.SetVector("lightColor", lightColor.GetColor());

        suzanne.Draw();

		/* lightCube.SetPosition({2.f,2.f, 6.f, 0.f}); */
        /* lightShader.UseShader(); */
        /* modelShader.SetVector("transform.position", lightCube.GetPosition()); */
        /* modelShader.SetVector("transform.axis_angle", lightCube.GetAxisAngle()); */
        /* lightShader.SetTransformationMatrix("view", view); */
        /* lightShader.SetTransformationMatrix("projection", projection); */

        /* lightCube.Draw(); */

        glfwSwapBuffers(m_window->GetGLFWWindow());
        glfwPollEvents();
    }
}

