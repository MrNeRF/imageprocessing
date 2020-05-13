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

Window* Viewer::Init(const std::string& name)
{
	m_spWindow = std::make_unique<Window>(name);
	return m_spWindow.get();
}

void Viewer::AddRenderObject(std::shared_ptr<IRenderable> spRenderObject)
{
	m_renderObjects.push_back(spRenderObject);
}

std::shared_ptr<IRenderable> Viewer::GetLastRenderObject()
{
	return m_renderObjects.back();
}

void Viewer::Run(void)
{
    auto& rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("Viewer::Run()");
	if (m_spWindow == nullptr)
	{
		rLogger.critical("No Window Object created!");
		ASSERT(0);
	}

    CHECK_GL_ERROR_(glEnable(GL_DEPTH_TEST));
    while (!glfwWindowShouldClose(m_spWindow->GetGLFWWindow()))
    {
        CHECK_GL_ERROR_(glClearColor(0.2f, 0.3f, 0.3f, 1.0f))
        CHECK_GL_ERROR_(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

		for (const auto& o : m_renderObjects)
		{
			o->Render();
		}

        glfwSwapBuffers(m_spWindow->GetGLFWWindow());
        glfwPollEvents();
    }
}

