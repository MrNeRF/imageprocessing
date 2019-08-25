#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Forward declaration callback method for resizing a window
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main(int, char **)
{

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::unique_ptr<GLFWwindow> window(glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rendering with OpenGL", NULL, NULL));

    if (window == nullptr)
    {
        std::cerr << "Failed to open GLFW window. \n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window.get());
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);

    std::cout << "Renderer: " << glGetString(GL_RENDERER);
    std::cout << "OpenGL version supported \n" << glGetString(GL_VERSION) << std::endl;
    glewExperimental = true;

    while(!glfwWindowShouldClose(window.get()))
    {

    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}