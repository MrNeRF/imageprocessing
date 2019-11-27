#include "Window.h"

Window::Window()
{
    // initialise glfw and m_glfwWindow,
    // create openGL context, initialise any other c++ resources
    glfwInit();
    m_glfwWindow = glfwCreateWindow(800, 600, "GL", NULL, NULL);

    // Userpointer -> Necessary to for casting
    glfwSetWindowUserPointer(m_glfwWindow, this);

    // Register Callbacks
    glfwSetFramebufferSizeCallback(m_glfwWindow, WindowResizeCallback);
    glfwSetMouseButtonCallback(m_glfwWindow, MouseInputCallback);
}

void Window::ViewPortResized(int width, int height)
{
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

void Window::WindowResizeCallback(GLFWwindow* win, int h, int w)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->ViewPortResized(w, h);
}

void Window::MouseInputCallback(GLFWwindow* win, int button, int action, int mods)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->MouseDeviceUpdate(button, action, mods);
}
