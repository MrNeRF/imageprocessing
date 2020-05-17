#include "Logger.h"
#include "Viewer.h"
#include "Window.h"
#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <spdlog/spdlog.h>
#include "Object3D.h"

int main()
{
    Logger::GetInstance().GetLogger().info("main()");

    std::shared_ptr<Camera> spCamera = std::make_shared<Camera>();
    spCamera->SetPerspectiveProjection(45.f, 800.f / 600.f, 0.1f, 50.f);

    auto viewer = Viewer();
    Window* pWindow = viewer.Init("OpenGL Window");
    
    std::shared_ptr<Shader> spLightShader = std::make_shared<Shader>("Light Cube");
    spLightShader->InitShaders("../Shaders/lightShader.vs", "../Shaders/lightShader.fs");
    std::shared_ptr<Light> spLightCube = std::make_shared<Light>();
    spLightCube->Init("../models/quader.obj", spCamera, spLightShader);

	viewer.AddRenderObject(spLightCube);
    std::shared_ptr<Shader> spModelShader = std::make_shared<Shader>("3DModelShader");
    spModelShader->InitShaders("../Shaders/modelShader.vs", "../Shaders/modelShader.fs");

    std::shared_ptr<Object3D> spSuzane = std::make_shared<Object3D>();
    spSuzane->Init("../models/suzanne.obj", spCamera, spModelShader);
	viewer.AddRenderObject(spSuzane);

    spSuzane->SetMaterial(Material::GetMaterial(MaterialType::GOLD));
    spSuzane->SetPosition({0.f, 0.f, 2.f, 0.f});
    spSuzane->SetLight(spLightCube);

	for (int i = 4; i < 8; ++i )
	{
		std::shared_ptr<Shader> spShader = std::make_shared<Shader>(std::string("3DModelShader") + std::to_string(i));
		spShader->InitShaders("../Shaders/modelShader.vs", "../Shaders/modelShader.fs");

		auto spRenderObject = std::make_shared<Object3D>();
		viewer.AddRenderObject(spRenderObject);

		spRenderObject->Init("../models/suzanne.obj", spCamera, spShader);
		spRenderObject->SetMaterial(Material::GetMaterial(MaterialType::GOLD));
		spRenderObject->SetPosition({(float)i, (float)i, 2.f + static_cast<float>(i - 4), 0.f});
		spRenderObject->SetLight(spLightCube);
		pWindow->attach(spRenderObject);
	}
    pWindow->attach(spCamera);
    pWindow->attach(spSuzane);

	// Render
    viewer.Run();
    return 0;
}


