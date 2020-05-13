#include <GL/glew.h>
///
#include <GLFW/glfw3.h>
#include "Object3D.h"
#include "File.h"
#include "Logger.h"
#include "Macros.h" 
#include "ObjFileParser.h"
#include <iostream>

void Object3D::Init(const std::string& pathToModel, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader)
{
    m_modelPath = pathToModel;
    m_spCamera  = spCamera;
    m_spShader  = spShader;
    
	Logger::GetInstance().GetLogger().info("ShaderName: {}", spShader->shaderName);
    m_spMesh3D = ObjFileParser().Parse(std::make_unique<File>(m_modelPath));
    if (m_spMesh3D != nullptr)
    {
        m_spOGLDataObject = std::make_unique<OpenGL3DDataObject>();
        m_spOGLDataObject->InitializeVertexBuffer(*m_spMesh3D);
        m_spOGLDataObject->InitializeNormalBuffer(*m_spMesh3D);
        m_spOGLDataObject->InitializeColorBuffer(m_vertexColor);

        /* if (spObjectMesh3D->HasUVCoordinates()) */
        /* { */
        /*     spOGLDataObject->InitializeTextureUVBuffer(spObjectMesh3D->uvCoordinates); */
        /* } */

    }
    else
    {
        assert(0);
    }
}

void Object3D::SetColor(const Color& color)
{
    m_vertexColor = color;
    m_spOGLDataObject->InitializeColorBuffer(m_vertexColor);
}

void Object3D::Render()
{
    m_spShader->UseShader();
    m_spShader->SetVector("transform.position", m_position);
    m_spShader->SetQuat("transform.qOrientation", m_orientation);
    m_spShader->SetQuat("transform.qconjOrientation", m_orientation.conjugate());


    m_spShader->SetTransformationMatrix("normalRotationMatrix", m_orientation.toRotationMatrix());
    m_spShader->SetVector("cameraPos", m_spCamera->GetCameraPosition());
    m_spShader->SetTransformationMatrix("view", m_spCamera->GetLookAt());
    m_spShader->SetTransformationMatrix("projection", m_spCamera->GetPerspectiveProjection());

    m_spShader->SetVector("light.ambient", Eigen::Vector3f(.5f, .5f, .5f));
    m_spShader->SetVector("light.diffuse", Eigen::Vector3f(.2f, .2f, .2f));
    m_spShader->SetVector("light.specular", Eigen::Vector3f(1.f, 1.f, 1.f));
    m_spShader->SetVector("light.position", m_spLight->GetPosition());

    if (m_spMaterial != nullptr)
    {
        m_spMaterial->Activate(m_spShader.get());

    }
    m_spOGLDataObject->DrawObject(GL_TRIANGLES);
}

void Object3D::onNotify(const EventType& eventType, IEvent* pEventData)
{
	switch(eventType)
	{
		case EventType::MOUSE_LEFT_BTN_DRAG:
		{
			MouseLeftBtnDragEvent* pMouseDragEvent = dynamic_cast<MouseLeftBtnDragEvent*>(pEventData);
			if(pMouseDragEvent != nullptr)
			{
				Eigen::Vector2f difference = (pMouseDragEvent->m_endCoordinates - pMouseDragEvent->m_startCoordinates).normalized();
				/* UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(5.f), */ 
				/* 			Eigen::Vector3f(-difference[1], difference[0], 0.f))); */
				UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(10.f), 
							Eigen::Vector3f(difference.y(), difference.x(), 0.f)));
			}
			else
			{
				// We should not get here -> BUG
				ASSERT(0);
			}
		}
		break;
		case EventType::KEY_PRESS:
		{
			KeyPressEvent* pKeyPressEvent = dynamic_cast<KeyPressEvent*>(pEventData);
			if(pKeyPressEvent != nullptr)
			{
				switch(pKeyPressEvent->m_key)
				{
					case GLFW_KEY_R:
						ResetRotation();
				}
			}
			else
			{
				// We should not get here -> BUG
				ASSERT(0);
			}
			break;
		}
		case EventType::MOUSE_MID_BTN_DRAG:
			[[fallthrough]];
		case EventType::MOUSE_CLICK:
			[[fallthrough]];
		case EventType::MOUSE_WHEEL:
			break;	
	}
}
