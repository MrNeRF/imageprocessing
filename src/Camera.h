#include <vector>
#include <GLFW/glfw3.h>

#define GLM_FORCE_CXX14
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class Camera
{
    public:
        Camera() = default;
        Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
            camPos{position}, worldUp{up} {};
        ~Camera() = default;
        void ProcessInput(GLFWwindow *window, float deltaTime);
        glm::mat4 GetViewMatrix(void) {return viewMatrix;};

    private:
        void updateView(float pitch, float yaw);
        void rotateCamera(float angle, float x, float y, float z);

    private:
        glm::vec3 camPos{0.0f, 0.0f, 3.0f};
        glm::vec3 camRight{1.0f, 0.0f, 0.0f};
        glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
        glm::vec3 camView{0.0f, 0.0f, -1.0f};
        glm::vec3 camUp = worldUp;

        glm::mat4 viewMatrix = glm::lookAt(camPos, camView, camUp);
        glm::quat cameraQuat = glm::toQuat(viewMatrix);
};

void Camera::rotateCamera(float angle, float x, float y, float z)
{
    glm::quat qview, qaxis;    

    qaxis.x = x * std::sin(angle * 0.5);
    qaxis.y = y * std::sin(angle * 0.5);
    qaxis.z = z * std::sin(angle * 0.5);
    qaxis.w = std::cos(angle * 0.5);

    qview.x = camView.x;
    qview.y = camView.y;
    qview.z = camView.z;

    qview = qaxis * qview * glm::conjugate(qaxis);

    camView.x = qview.x;
    camView.y = qview.y;
    camView.z = qview.z;
}

void Camera::updateView(float pitch, float yaw)
{
    glm::vec3 axis = glm::normalize(glm::cross(camView + camPos, camUp));
    rotateCamera(yaw, worldUp.x, worldUp.y, worldUp.z);
    rotateCamera(pitch, axis.x, axis.y, axis.z);
    camRight = glm::normalize(glm::cross(camView, worldUp));
    camUp = glm::normalize(glm::cross(camRight, camView));
    viewMatrix = glm::lookAt(camPos, camPos + camView, camUp);
}

void Camera::ProcessInput(GLFWwindow *window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    float cameraSpeed = 2.5 * deltaTime; 
    float yaw = 0.0f; 
    float pitch = 0.0f;
    float sensitivity = 5.0;

    // Translation
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        yaw += cameraSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        yaw -= cameraSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        pitch -= cameraSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
    {
        pitch += cameraSpeed;
    }

// Rotation
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camPos += camView * cameraSpeed * sensitivity;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        camPos -= camView * cameraSpeed * sensitivity;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camPos -= camRight * cameraSpeed * sensitivity;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        camPos += camRight * cameraSpeed * sensitivity;
    }
    updateView(pitch, yaw);
}