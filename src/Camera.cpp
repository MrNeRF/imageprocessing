#include "Camera.h"
#include <cmath>
#include <iostream>

Eigen::Matrix4f Camera::LookAt(Eigen::Vector3f& eye, Eigen::Vector3f& target, Eigen::Vector3f& up)
{
    // Right Hand Coordinate System
    Eigen::Vector3f camDir   = (eye - target).normalized();
    Eigen::Vector3f camRight = (up.cross(camDir)).normalized();
    Eigen::Vector3f camUp    = camDir.cross(camRight);

    Eigen::Matrix<float, 4, 4, Eigen::RowMajor> view = Eigen::Matrix4f::Zero(4, 4);
    view.block(0, 0, 1, 3)                           = camRight.transpose();
    view.block(1, 0, 1, 3)                           = camUp.transpose();
    view.block(2, 0, 1, 3)                           = camDir.transpose();
    view(0, 3)                                       = -camRight.dot(eye);
    view(1, 3)                                       = -camUp.dot(eye);
    view(2, 3)                                       = -camDir.dot(eye);
    view(3, 3)                                       = 1.f;

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
