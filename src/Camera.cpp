#include "Camera.h"
#include <cmath>
#include "Macros.h"
#include "Logger.h"
#include <iostream>


static Eigen::Matrix4f Frustum(float left, float right, float bottom, float top, float near, float far);

const Eigen::Matrix4f& Camera::GetLookAt() const
{
    // Right Hand Coordinate System
    Eigen::Vector3f camDir   = (m_eye - m_target).normalized();
    Eigen::Vector3f camRight = (m_up.cross(camDir)).normalized();
    Eigen::Vector3f camUp    = camDir.cross(camRight);

    m_view.block(0, 0, 1, 3)                           = camRight.transpose();
    m_view.block(1, 0, 1, 3)                           = camUp.transpose();
    m_view.block(2, 0, 1, 3)                           = camDir.transpose();
    m_view(0, 3)                                       = -camRight.dot(m_eye);
    m_view(1, 3)                                       = -camUp.dot(m_eye);
    m_view(2, 3)                                       = -camDir.dot(m_eye);
    m_view(3, 3)                                       = 1.f;

    return m_view;
}

void Camera::SetPerspectiveProjection(float fov, float aspectRatio, float zNearPlane, float zFarPlane)
{
    float height = zNearPlane * tanf(fov * .5f);
    float width  = height * aspectRatio;

    m_frustum = Frustum(-width, width, -height, height, zNearPlane, zFarPlane);
}

const Eigen::Matrix4f& Camera::GetPerspectiveProjection() const
{ 
	ASSERT(!m_frustum.isZero());
	return m_frustum; 
}

void Camera::onNotify(const EventType &eventType, IEvent* pEventData)
{
	if(eventType == EventType::MOUSEWHEEL)
	{

		MouseWheelEvent* pMouseWheelEvent = dynamic_cast<MouseWheelEvent*>(pEventData);
		if(pMouseWheelEvent != nullptr)
		{
			m_eye.z() += -0.1f * static_cast<float>(pMouseWheelEvent->m_yoffset);
		}
		else
		{
			ASSERT(0);
		}
	}
}

Eigen::Matrix4f Frustum(float left, float right, float bottom, float top, float near, float far)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Zero(4, 4);

    projection(0, 0) = 2.f * near / (right - left);
    projection(0, 2) = (right + left) / (right - left);
    projection(1, 1) = 2.f * near / (top - bottom);
    projection(1, 2) = (top + bottom) / (top - bottom);
    projection(2, 2) = -(far + near) / (far - near);
    projection(2, 3) = -(2.f * far * near) / (far - near);
    projection(3, 2) = -1.f;

    return projection;
}
