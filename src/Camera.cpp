#include "Camera.h"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI (4.0 * std::atan2(1.0, 1.0))
#endif

template<typename N>
inline N deg2rad(N d)
{
    return M_PI * d / 180.0;
}

static Eigen::Matrix4f Frustum(float left, float right, float bottom, float top, float near, float far);

Eigen::Matrix4f Camera::LookAt(Eigen::Vector3f& eye, Eigen::Vector3f& target, Eigen::Vector3f& up)
{
    // Right Hand Coordinate System
    Eigen::Vector3f camDir   = (eye - target).normalized();
    Eigen::Vector3f camRight = (up.cross(camDir)).normalized();
    Eigen::Vector3f camUp    = camDir.cross(camRight);

    Eigen::Matrix4f view                             = Eigen::Matrix4f::Zero(4, 4);
    view.block(0, 0, 1, 3)                           = camRight.transpose();
    view.block(1, 0, 1, 3)                           = camUp.transpose();
    view.block(2, 0, 1, 3)                           = camDir.transpose();
    view(0, 3)                                       = -camRight.dot(eye);
    view(1, 3)                                       = -camUp.dot(eye);
    view(2, 3)                                       = -camDir.dot(eye);
    view(3, 3)                                       = 1.f;

    return view;
}

Eigen::Matrix4f Camera::PerspectiveProjection(float fov, float aspectRatio, float zNearPlane, float zFarPlane)
{
    float height = zNearPlane * tanf(fov * .5f);
    float width  = height * aspectRatio;

    return Frustum(-width, width, -height, height, zNearPlane, zFarPlane);
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
