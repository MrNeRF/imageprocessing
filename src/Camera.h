#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include "IObserver.h"

class Camera : public IObserver
{
    public:
        Camera() = default;
        explicit Camera(Eigen::Vector3f& eye, Eigen::Vector3f& target, Eigen::Vector3f& up) : m_eye{eye}, m_target{target}, m_up{up}{};
        const Eigen::Matrix4f& GetLookAt() const;
        void                   SetPerspectiveProjection(float fov, float aspectRatio, float zNearPlane, float zFarPlane);
        const Eigen::Matrix4f& GetPerspectiveProjection() const; 
        const Eigen::Vector3f& GetCameraPosition() const {return m_eye;}

        // Observer
        void onNotify(const EventType& eventType, IEvent* pEventData) override;

    private:
        Eigen::Vector3f         m_eye{0.f, 0.f, 5.f};
        Eigen::Vector3f         m_target{0.f, 0.f, 0.f};
        Eigen::Vector3f         m_up{0.f, 1.f, 0.f};
        Eigen::Matrix4f         m_frustum;
        mutable Eigen::Matrix4f m_view;
};

#endif
