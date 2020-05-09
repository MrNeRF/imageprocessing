#include "Viewer.h"
#include "Camera.h"
#include "Color.h"
#include "Macros.h"
#include "Object3D.h"
#include "Shader.h"
#include <memory>
#include "Window.h"
#include <GLFW/glfw3.h>
#include "Material.h"
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
	std::shared_ptr<Shader> spModelShader = std::make_shared<Shader>("3D Model Shader");
    spModelShader->InitShaders("../Shaders/modelShader.vs", "../Shaders/modelShader.fs");
    Shader lightShader("Light Cube");
    lightShader.InitShaders("../Shaders/lightShader.vs", "../Shaders/lightShader.fs");
    lightShader.UseShader();
    Eigen::Vector3f lightPos = Eigen::Vector3f(10.f, 10.f, 0.0f);
    Color           lightColor(1.f, 1.f, 1.f);
    lightShader.SetVector("lightPosition", lightPos);
	std::shared_ptr<Camera> spCamera = std::make_shared<Camera>();
	m_window->attach(spCamera);

    glEnable(GL_DEPTH_TEST);
	std::shared_ptr<Object3D> suzanne = std::make_shared<Object3D>("../models/suzanne.obj");
    m_window->attach(suzanne);
    Object3D lightCube("../models/quader.obj");
    lightCube.SetColor(lightColor);


    while (!glfwWindowShouldClose(m_window->GetGLFWWindow()))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_window->GetGLFWWindow(), true);
        }

        /* if (m_window->keyboardDevice.key == GLFW_KEY_W */
        /*     && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS) */
        /* { */
        /*     suzanne.UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(5.f), Eigen::Vector3f::UnitX())); */
        /* } */
        /* if (m_window->keyboardDevice.key == GLFW_KEY_S */
        /*     && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS) */
        /* { */
        /*     suzanne.UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(-5.f), Eigen::Vector3f::UnitX())); */
        /* } */
        /* if (m_window->keyboardDevice.key == GLFW_KEY_D */
        /*     && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS) */
        /* { */
        /*     suzanne.UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(5.f), Eigen::Vector3f::UnitY())); */
        /* } */
        /* if (m_window->keyboardDevice.key == GLFW_KEY_A */
        /*     && m_window->keyboardDevice.currentAction == Window::KeyboardDevice::KeyAction::PRESS) */
        /* { */
        /*     suzanne.UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(-5.f), Eigen::Vector3f::UnitY())); */
        /* } */

        if (m_window->m_key == GLFW_KEY_R
            && m_window->m_bKeyPressed == true)
        {
        	suzanne->ResetRotation();
        }

        spModelShader->UseShader();
        const Eigen::Matrix4f view  = spCamera->GetLookAt();
        const Eigen::Matrix4f projection = spCamera->PerspectiveProjection(45.f, 800.f / 600.f, 0.1f, 50.f);

		suzanne->SetMaterial(MaterialFactory().GetMaterial(MaterialType::GOLD));
		suzanne->SetPosition({0.f,0.f, 2.f, 0.f});
        spModelShader->SetVector("transform.position", suzanne->GetPosition());
        spModelShader->SetQuat("transform.qOrientation", suzanne->GetOrientation());
        spModelShader->SetQuat("transform.qconjOrientation", suzanne->GetOrientation().conjugate());

        spModelShader->SetTransformationMatrix("cameraPos", spCamera->GetCameraPosition());
        spModelShader->SetTransformationMatrix("view", view);
        spModelShader->SetTransformationMatrix("projection", projection);

        spModelShader->SetVector("light.ambient", Eigen::Vector3f(.5f, .5f, .5f));
        spModelShader->SetVector("light.diffuse", Eigen::Vector3f(.2f, .2f, .2f));
        spModelShader->SetVector("light.specular", Eigen::Vector3f(1.f, 1.f, 1.f));

        spModelShader->SetVector("lightPosition", lightPos);
        spModelShader->SetVector("lightColor", lightColor.GetColor());

        suzanne->Draw(spModelShader);

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

