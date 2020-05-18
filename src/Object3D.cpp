#include <GL/glew.h>
///
#include <GLFW/glfw3.h>
#include "Object3D.h"
#include "File.h"
#include "Logger.h"
#include "Macros.h" 
#include "ObjFileParser.h"
#include <iostream>
#include <numeric>

void Object3D::Init(std::shared_ptr<Mesh3D> spMesh3D, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader)
{
	Logger::GetInstance().GetLogger().info("Object3D: {}", m_name);
	Logger::GetInstance().GetLogger().info("ShaderName: {}", spShader->shaderName);
    m_spMesh3D  = spMesh3D;
    m_spCamera  = spCamera;
    m_spShader  = spShader;
    
    if (m_spMesh3D != nullptr)
    {
        m_spOGLDataObject = std::make_unique<OpenGL3DDataObject>();
        m_spOGLDataObject->InitializeVertexBuffer(*m_spMesh3D);
        m_spOGLDataObject->InitializeNormalBuffer(*m_spMesh3D);
        SetColor(m_vertexColor);
    }
    else
    {
    	ASSERT(0);
    }
}

void Object3D::SetColor(const Color& color)
{
    m_vertexColor = color;
    m_spOGLDataObject->InitializeColorBuffer(m_vertexColor);
}

void Object3D::UpdateNormalBuffer()
{
    m_spOGLDataObject->InitializeNormalBuffer(*m_spMesh3D);
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

bool Object3D::rayTriangleIntersection(const Eigen::Vector2f& clickedPoint)
{
	Eigen::Matrix3f	model = m_orientation.toRotationMatrix();
	Eigen::Matrix4f model4f = Eigen::Matrix4f::Zero();
	model4f.block(0,0,3,3) = model;

	model4f.col(3) = m_position;
	model4f(3,3) = 1.f;
	
	Eigen::Matrix4f modelView = m_spCamera->GetLookAt() * model4f;
	Eigen::Matrix4f MPVinv = (m_spCamera->GetPerspectiveProjection() * modelView).inverse();
	auto click = Eigen::Vector4f(clickedPoint.x(), clickedPoint.y(), 0.f, 1.f);
	click[0] = 2.f * click[0] / 600.f - 1;
	click[1] = 1 - 2.f * click[1] / 800.f;
	Eigen::Vector4f nearPoint = MPVinv* click;
	Eigen::Vector4f cameraPos4f = modelView.inverse().col(3);
	Eigen::Vector4f dir4f = nearPoint - cameraPos4f;
	Eigen::Vector3f camerPos =  m_spCamera->GetCameraPosition();
	Eigen::Vector3f dir = Eigen::Vector3f(dir4f.x(), dir4f.y(), dir4f.z());
	bool bHit = false;
	for(const auto& triangleIndex : faceIterator(*m_spMesh3D))
	{
		auto [vec0, vec1, vec2] = m_spMesh3D->GetFaceVertices(triangleIndex);
		/* vec0 = model * vec0 + position; */
		/* vec1 = model * vec1 + position; */
		/* vec2 = model * vec2 + position; */
		Eigen::Vector3f vecA = vec1 - vec0;
		Eigen::Vector3f vecB = vec2 - vec0;
		Eigen::Vector3f pVec = dir.cross(vecB);
		float det = vecA.dot(pVec);
		if (det < std::numeric_limits<float>::epsilon())
		{
			continue;
		}
		float invDet = 1.f / det;
		Eigen::Vector3f tvec = camerPos - vec0;
		float u = tvec.dot(pVec) * invDet;
		if (u < 0.f || u > 1.f)
		{
			continue;
		}

		Eigen::Vector3f qvec = tvec.cross(vecA);
		float v = dir.dot(qvec) * invDet;
		if ((v < 0) || u + v > 1.f)
		{
			continue;
		}
		bHit = true;
		break;
	}
	if (bHit)
	{
		std::cout << "Treffer\n";
		return true;
	}
	return false;
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
		}
		break;
		case EventType::MOUSE_MID_BTN_DRAG:
		{
			MouseMidBtnDragEvent* pMouseDragEvent = dynamic_cast<MouseMidBtnDragEvent*>(pEventData);
			if(pMouseDragEvent != nullptr)
			{
				Eigen::Vector2f difference = (pMouseDragEvent->m_endCoordinates - pMouseDragEvent->m_startCoordinates).normalized();
				difference *= 0.4f;
				auto pos = GetPosition(); 
				pos[0] += difference.x();
				pos[1] += difference.y();
				SetPosition(pos);
			}
			else
			{
				// We should not get here -> BUG
				ASSERT(0);
			}
		}
		break;
		case EventType::MOUSE_LEFT_BTN_CLICK:
		{
			MouseLeftBtnClickEvent* pMouseLeftBtnClickEvent = dynamic_cast<MouseLeftBtnClickEvent*>(pEventData);
			if(pMouseLeftBtnClickEvent != nullptr)
			{
				Eigen::Vector2f clickedPosition = pMouseLeftBtnClickEvent->m_point;
				rayTriangleIntersection(clickedPosition);
			}
			else
			{
				// We should not get here -> BUG
				ASSERT(0);
			}
		}
		break;
		case EventType::MOUSE_WHEEL:
			break;	
	}
}
