#include "Logger.h"
#include "Viewer.h"
#include "Window.h"
#include "Camera.h"
#include "File.h"
#include "ObjFileParser.h"
#include "Light.h"
#include "Shader.h"
#include "Quader.h"
#include "AlgoVertexNormals.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <spdlog/spdlog.h>
#include "Object3D.h"

int main()
{
    Logger::GetInstance().GetLogger().info("main()");

    auto viewer = Viewer();
    Window* pWindow = viewer.Init("OpenGL Window");
    std::shared_ptr<Camera> spCamera = std::make_shared<Camera>();
    spCamera->SetPerspectiveProjection(45.f, pWindow->GetAspectRatio(), 1.f, 50.f);

    
    std::shared_ptr<Shader> spLightShader = std::make_shared<Shader>("Light Cube");
    spLightShader->InitShaders("../Shaders/lightShader.vs", "../Shaders/lightShader.fs");
    std::shared_ptr<Light> spLightCube = std::make_shared<Light>("Light");
	std::shared_ptr<Mesh3D> spLightMesh3D = ObjFileParser().Parse(std::make_unique<File>("../models/quader.obj"));
    spLightCube->Init(spLightMesh3D, spCamera, spLightShader);

	viewer.AddRenderObject(spLightCube);
    std::shared_ptr<Shader> spModelShader = std::make_shared<Shader>("3DModelShader");
    spModelShader->InitShaders("../Shaders/modelShader.vs", "../Shaders/modelShader.fs");

	std::shared_ptr<Mesh3D> spSuzaneMesh3D = ObjFileParser().Parse(std::make_unique<File>("../models/suzanne.obj"));
    std::shared_ptr<Object3D> spSuzane = std::make_shared<Object3D>("Suzanne");
    spSuzane->Init(spSuzaneMesh3D, spCamera, spModelShader);
	/* auto algo = AlgoVertexNormals(*spSuzaneMesh3D); */
	/* bool bOK = algo.Compute(); */
	/* spSuzane->UpdateNormalBuffer(); */
    /* ASSERT(bOK); */

    spSuzane->SetMaterial(Material::GetMaterial(MaterialType::GOLD));
    spSuzane->SetPosition({0.f, 0.f, -5.f, 0.f});
    spSuzane->SetLight(spLightCube);
	/* viewer.AddRenderObject(spSuzane); */

		
    std::shared_ptr<Shader> spQuaderShader = std::make_shared<Shader>("Quader Shader");
    spModelShader->InitShaders("../Shaders/modelShader.vs", "../Shaders/modelShader.fs");
	Quader quader(1.f, 1.f, 1.f);
	
	std::shared_ptr<Mesh3D> spQuaderMesh = quader.GetMesh();
	auto algo = AlgoVertexNormals(*spQuaderMesh);
	bool bOK = algo.Compute();
	ASSERT(bOK);
    std::shared_ptr<Object3D> spBox = std::make_shared<Object3D>("Box");
    spBox->Init(spQuaderMesh, spCamera, spModelShader);
    spBox->SetMaterial(Material::GetMaterial(MaterialType::GOLD));
    spBox->SetPosition({0.f, 0.f, 2.f, 0.f});
    spBox->SetLight(spLightCube);
    viewer.AddRenderObject(spBox);

    pWindow->attach(spCamera);
    /* pWindow->attach(spSuzane); */
    pWindow->attach(spBox);
	std::shared_ptr<Shader> spShader = std::make_shared<Shader>(std::string("3DModelShader") + std::to_string(2));
	spShader->InitShaders("../Shaders/modelShader.vs", "../Shaders/modelShader.fs");

	std::shared_ptr<Object3D> spRenderObject = std::make_shared<Object3D>(std::string("Suzanne") + std::to_string(2));
	spRenderObject->Init(ObjFileParser().Parse(std::make_unique<File>("../models/suzanne.obj")), spCamera, spShader);
	spRenderObject->SetMaterial(Material::GetMaterial(MaterialType::GOLD));
	spRenderObject->SetPosition({2.f, 0.f, 2.f, 0.f});
	spRenderObject->SetLight(spLightCube);
	viewer.AddRenderObject(spRenderObject);
	pWindow->attach(spRenderObject);


	/* spQuaderMesh->IterateAllFaces(); */	
	/* std::cout << "\nVertex 0: "; */ 
	/* for(const auto& faceID : oneRingFaceIterator(*spQuaderMesh, 0)) */
	/* { */
	/* 	std::cout << faceID << ", "; */
	/* } */
	/* std::cout << "\nVertex 1: "; */ 
	/* for(const auto& faceID : oneRingFaceIterator(*spQuaderMesh, 1)) */
	/* { */
	/* 	std::cout << faceID << ", "; */
	/* } */
	/* std::cout << "\nVertex 2: "; */ 
	/* for(const auto& faceID : oneRingFaceIterator(*spQuaderMesh, 2)) */
	/* { */
	/* 	std::cout << faceID << ", "; */
	/* } */
	/* std::cout << "\nVertex 3: "; */ 
	/* for(const auto& faceID : oneRingFaceIterator(*spQuaderMesh, 3)) */
	/* { */
	/* 	std::cout << faceID << ", "; */
	/* } */
	/* std::cout << "\nVertex 4: "; */ 
	/* for(const auto& faceID : oneRingFaceIterator(*spQuaderMesh, 4)) */
	/* { */
	/* 	std::cout << faceID << ", "; */
	/* } */
	/* std::cout << "\nVertex 5: "; */ 
	/* for(const auto& faceID : oneRingFaceIterator(*spQuaderMesh, 5)) */
	/* { */
	/* 	std::cout << faceID << ", "; */
	/* } */
	/* std::cout << "\nVertex 6: "; */ 
	/* for(const auto& faceID : oneRingFaceIterator(*spQuaderMesh, 6)) */
	/* { */
	/* 	std::cout << faceID << ", "; */
	/* } */
	/* std::cout << "\nVertex 7: "; */ 
	/* for(const auto& faceID : oneRingFaceIterator(*spQuaderMesh, 7)) */
	/* { */
	/* 	std::cout << faceID << ", "; */
	/* } */
	/* std::cout << "\n"; */ 
	// Render
    viewer.Run();
    return 0;
}


