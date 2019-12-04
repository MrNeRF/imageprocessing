#include <Eigen/Dense>

class Camera
{
    public:
        Camera() = default;

        static Eigen::Matrix4f LookAt(Eigen::Vector3f& eye, Eigen::Vector3f& target, Eigen::Vector3f& up);
        static Eigen::Matrix4f PerspectiveProjection(float fov, float aspectRatio, float zNearPlane, float zFarPlane);
};

