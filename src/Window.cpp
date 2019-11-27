#include <GL/glew.h>

#include "Window.h"
#include <cstdio>
#include <iostream>

Window::Window(const std::string name)
    : windowName{name}
{
    // initialise glfw and m_windowInstance,
    // create openGL context, initialise any other c++ resources
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    windowInstance = glfwCreateWindow(800, 600, "Window", NULL, NULL);

    if (windowInstance == NULL)
    {
        std::puts("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(windowInstance);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::puts("Failed to initialize GLEW\n");
        glfwTerminate();
        return;
    }
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version  = glGetString(GL_VERSION);  // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported \n"
              << version << std::endl;
    // Userpointer -> Necessary to for casting
    glfwSetWindowUserPointer(windowInstance, this);

    // Register Callbacks
    glfwSetFramebufferSizeCallback(windowInstance, WindowResizeCallback);
    glfwSetMouseButtonCallback(windowInstance, MouseInputCallback);
}

Window::~Window(void)
{
    glfwTerminate();
}

void Window::ViewPortResized(int width, int height)
{
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

void Window::MouseDeviceUpdate(GLFWwindow* win, int button, int action, int mods)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseDevice.currentButton = MouseDevice::MouseButton::LEFTMOUSEBUTTON;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mouseDevice.currentButton = MouseDevice::MouseButton::MIDDLEMOUSEBUTTON;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mouseDevice.currentButton = MouseDevice::MouseButton::RIGHTMOUSEBUTTON;
            break;
        default:
            mouseDevice.currentButton = MouseDevice::MouseButton::NONE;
    }

    switch (action)
    {
        case GLFW_RELEASE:
            mouseDevice.currentAction = MouseDevice::MouseAction::RELEASE;
            break;
        case GLFW_PRESS:
            mouseDevice.currentAction = MouseDevice::MouseAction::PRESS;
            break;
        case GLFW_REPEAT:
            mouseDevice.currentAction = MouseDevice::MouseAction::REPEAT;
            break;
    }

    glfwGetCursorPos(win, &mouseDevice.xPos, &mouseDevice.yPos);
    //@TODO Modifier Keys alt, shift, etc...
}

void Window::WindowResizeCallback(GLFWwindow* win, int h, int w)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->ViewPortResized(w, h);
}

void Window::MouseInputCallback(GLFWwindow* win, int button, int action, int mods)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->MouseDeviceUpdate(win, button, action, mods);
}
