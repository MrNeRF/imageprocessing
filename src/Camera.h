#include <Eigen/Dense>
#include "IObserver.h"

class Camera : public IObserver
{
    public:
        Camera() = default;
        explicit Camera(Eigen::Vector3f& eye, Eigen::Vector3f& target, Eigen::Vector3f& up) : m_eye{eye}, m_target{target}, m_up{up}{};
        const Eigen::Matrix4f& GetLookAt() const;
        Eigen::Matrix4f PerspectiveProjection(float fov, float aspectRatio, float zNearPlane, float zFarPlane);
        const Eigen::Vector3f& GetCameraPosition() {return m_eye;}

        // Observer
		void onNotify(const EventType &eventType, IEvent* pEventData) override;
	private:
		Eigen::Vector3f m_eye{0.f, 0.f, 5.f};
		Eigen::Vector3f m_target{0.f, 0.f, 0.f};
		Eigen::Vector3f m_up{0.f, 1.f, 0.f};
		mutable Eigen::Matrix4f m_view;
};

