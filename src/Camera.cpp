#include "Camera.h"
#include <cmath>

Eigen::Matrix4f Camera::LookAt(Eigen::Vector3f& eye, Eigen::Vector3f& target, Eigen::Vector3f& up)
{
    // Right Hand Coordinate System
    Eigen::Vector3f zAxis = (eye - target).normalized();
    Eigen::Vector3f xAxis = (up.cross(zAxis)).normalized();
    Eigen::Vector3f yAxis = zAxis.cross(xAxis);
    up                    = yAxis;

    Eigen::Matrix4f view   = Eigen::Matrix4f::Zero(4, 4);
    view.block(0, 0, 3, 1) = xAxis;
    view.block(0, 1, 3, 1) = yAxis;
    view.block(0, 2, 3, 1) = zAxis;
    view(3, 0)             = xAxis.dot(eye);
    view(3, 1)             = xAxis.dot(eye);
    view(3, 2)             = xAxis.dot(eye);

    return view;
}

Eigen::Matrix4f Camera::PerspectiveProjection(float fov, float aspectRation, float zNearPlane, float zFarPlane)
{
    float           tanfFovHalf = tanf(fov * .5f);
    Eigen::Matrix4f projection  = Eigen::Matrix4f::Zero(4, 4);

    projection(0, 0) = 1.f / (aspectRation * tanfFovHalf);
    projection(1, 1) = 1.f / (tanfFovHalf);
    projection(2, 2) = zFarPlane / (zNearPlane - zFarPlane);
    projection(2, 3) = -1.f;
    projection(3, 2) = -(zFarPlane * zNearPlane) / (zFarPlane - zNearPlane);

    return projection;
}
