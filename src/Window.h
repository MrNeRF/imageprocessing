#ifndef WINDOW_H
#define WINDOW_H

#include "GLFW/glfw3.h"
#include "MouseDevice.h"

class Window
{
public:
    Window();

    void ViewPortResized(int width, int height);
    void MouseDeviceUpdate(int button, int action, int mods);

public:
    // Callbacks
    static void WindowResizeCallback(GLFWwindow* win, int h, int w);
    static void MouseInputCallback(GLFWwindow* win, int button, int action, int mods);

private:
    int         m_winWidth   = 800;
    int         m_winHeight  = 600;
    GLFWwindow* m_glfwWindow = nullptr;
    MouseDevice m_MouseDevice;
};

#endif
