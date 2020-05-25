#include <GL/glew.h>
///
#include "File.h"
#include "Logger.h"
#include "Macros.h"
#include "ObjFileParser.h"
#include "Object3D.h"
#include "Ray.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <numeric>

void Object3D::Init(std::shared_ptr<Mesh3D> spMesh3D, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader)
{
    Logger::GetInstance().GetLogger().info("Object3D: {}", m_name);
    Logger::GetInstance().GetLogger().info("ShaderName: {}", spShader->shaderName);
    m_spMesh3D = spMesh3D;
    m_spCamera = spCamera;
    m_spShader = spShader;

    if (m_spMesh3D != nullptr)
    {
        m_spOGLDataObject = std::make_unique<OpenGL3DDataObject>();
        m_spOGLDataObject->InitializeVertexBuffer(*m_spMesh3D);
        m_spOGLDataObject->InitializeNormalBuffer(*m_spMesh3D);
        SetColor(m_vertexColor);
        m_spBVolume = std::make_unique<BoundingVolume>(BoundingVolume::EBoundingVolume::Cube, *m_spMesh3D);
    }
    else
    {
        ASSERT(0);
    }
}

void Object3D::SetColor(const Color& color)
{
    m_vertexColor = color;
    std::vector<Eigen::Vector3f> colorData(m_spMesh3D->GetNumberOfVertice(), color.GetColor());
    m_spOGLDataObject->InitializeColorBuffer(colorData);
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

	Eigen::Matrix4f model =Eigen::Matrix4f::Identity();
	model(0,3) = m_position.x();
	model(1,3) = m_position.y();
	model(2,3) = m_position.z();

	m_spBVolume->Draw(model, *m_spCamera);
}

bool Object3D::rayTriangleIntersection(const Eigen::Vector2f& clickedPoint, float windowWidth, float windowHeight)
{
    Eigen::Vector2f NDC;
    NDC[0] = 2.f * clickedPoint[0] / windowWidth;
    NDC[1] = 2.f * clickedPoint[1] / windowHeight;
    Eigen::Vector2f PixelScreen;
    PixelScreen(0) =  NDC[0] - 1;
    PixelScreen(1) = 1 -  NDC[1];

    Eigen::Vector4f ray_clipped = Eigen::Vector4f(PixelScreen[0], PixelScreen[1], -1, 1);
    Eigen::Vector4f ray_eye = m_spCamera->GetPerspectiveProjection().inverse() * ray_clipped;
    ray_eye = Eigen::Vector4f(ray_eye.x(), ray_eye.y(), -1.f, 0.f);

    Eigen::Vector4f ray_world4f = (m_spCamera->GetLookAt().inverse() * ray_eye);
	Eigen::Vector3f ray_world   = Eigen::Vector3f(ray_world4f.x(), ray_world4f.y(), ray_world4f.z()).normalized();
    Eigen::Vector3f camPos      = m_spCamera->GetCameraPosition();

    std::vector<Eigen::Vector3f> vertices;
    vertices.push_back(camPos);
    vertices.push_back(10.f * ray_world);
    std::vector<uint32_t> indices{0, 1};

    Eigen::Matrix3f model     = m_orientation.toRotationMatrix();
    Eigen::Vector3f position  = Eigen::Vector3f(m_position.x(), m_position.y(), m_position.z());


	Ray ray(camPos, ray_world, 0.f);
    if(!m_spBVolume->IsBHHit(ray, position))
	{
		return false;
	}
    for (const auto& triangleIndex : faceIterator(*m_spMesh3D))
    {
        auto [vec0, vec1, vec2] = m_spMesh3D->GetFaceVertices(triangleIndex);
        vec0                    = model * vec0 + position;
        vec1                    = model * vec1 + position;
        vec2                    = model * vec2 + position;
        Eigen::Vector3f vecA    = vec1 - vec0;
        Eigen::Vector3f vecB    = vec2 - vec0;
        Eigen::Vector3f pVec    = ray_world.cross(vecB);
        float           det     = vecA.dot(pVec);
        if (std::abs(det) < std::numeric_limits<float>::epsilon())
        {
            continue;
        }
        float           invDet = 1.f / det;
        Eigen::Vector3f tvec   = camPos - vec0;
        float           u      = tvec.dot(pVec) * invDet;
        if (u < 0.f || u > 1.f)
        {
            continue;
        }

        Eigen::Vector3f qvec = tvec.cross(vecA);
        float           v    = ray_world.dot(qvec) * invDet;
        if ((v < 0) || u + v > 1.f)
        {
            continue;
        }
    	return true;
    }
    return false;
}

void Object3D::onNotify(const EventType& eventType, IEvent* pEventData)
{
    switch (eventType)
    {
        case EventType::MOUSE_LEFT_BTN_DRAG:
        {
            MouseLeftBtnDragEvent* pMouseDragEvent = dynamic_cast<MouseLeftBtnDragEvent*>(pEventData);
            if (pMouseDragEvent != nullptr)
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
            if (pKeyPressEvent != nullptr)
            {
                switch (pKeyPressEvent->m_key)
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
            if (pMouseDragEvent != nullptr)
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
            if (pMouseLeftBtnClickEvent != nullptr)
            {
                rayTriangleIntersection(pMouseLeftBtnClickEvent->m_point, pMouseLeftBtnClickEvent->m_windowWidth, pMouseLeftBtnClickEvent->m_windowHeigth);
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
