#include "Viewer.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Logger.h"
#include "Macros.h"
#include "Material.h"
#include "Object3D.h"
#include "Shader.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <memory>

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
    auto& logger = Logger::GetInstance().GetLogger();
    logger.info("Viewer::Run()");

    std::shared_ptr<Camera> spCamera = std::make_shared<Camera>();
    m_window->attach(spCamera);
	

    std::shared_ptr<Shader> spLightShader = std::make_shared<Shader>("Light Cube");
    spLightShader->InitShaders("../Shaders/lightShader.vs", "../Shaders/lightShader.fs");
    std::shared_ptr<Light> spLightCube = std::make_shared<Light>();
    spLightCube->Init("../models/quader.obj", spCamera, spLightShader);

    std::shared_ptr<Shader> spModelShader = std::make_shared<Shader>("3DModelShader");
    spModelShader->InitShaders("../Shaders/modelShader.vs", "../Shaders/modelShader.fs");
    std::shared_ptr<Object3D> suzanne = std::make_shared<Object3D>();
    suzanne->Init("../models/suzanne.obj", spCamera, spModelShader);

    suzanne->SetMaterial(Material::GetMaterial(MaterialType::GOLD));
    suzanne->SetPosition({0.f, 0.f, 2.f, 0.f});
    suzanne->SetLight(spLightCube);

    m_window->attach(suzanne);

    CHECK_GL_ERROR_(glEnable(GL_DEPTH_TEST));
    while (!glfwWindowShouldClose(m_window->GetGLFWWindow()))
    {
        CHECK_GL_ERROR_(glClearColor(0.2f, 0.3f, 0.3f, 1.0f))
        CHECK_GL_ERROR_(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

        if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_window->GetGLFWWindow(), true);
        }

        if (m_window->m_key == GLFW_KEY_R
            && m_window->m_bKeyPressed == true)
        {
        	suzanne->ResetRotation();
        }

        spLightCube->Render();
        suzanne->Render();
        glfwSwapBuffers(m_window->GetGLFWWindow());
        glfwPollEvents();
    }
}

