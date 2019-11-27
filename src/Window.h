#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include "GLFW/glfw3.h"
#include <cstdint>
#include <string>

class Window
{
public:
    struct MouseDevice
    {
        enum class MouseButton
        {
            NONE,
            LEFTMOUSEBUTTON,
            MIDDLEMOUSEBUTTON,
            RIGHTMOUSEBUTTON
        };

        enum class MouseAction
        {
            RELEASE,
            PRESS,
            REPEAT
        };

        double   xPos       = 0;
        double   yPos       = 0;
        bool     isPressed  = false;
        bool     isReleased = true;

        MouseButton currentButton = MouseButton::NONE;
        MouseAction currentAction = MouseAction::RELEASE;
    };

public:
    Window(const std::string name);
    ~Window(void);

    void ViewPortResized(int width, int height);
    void MouseDeviceUpdate(GLFWwindow* win, int button, int action, int mods);
    GLFWwindow* GetGLFWWindow(void) { return windowInstance; }
    // Callbacks
    static void WindowResizeCallback(GLFWwindow* win, int h, int w);
    static void MouseInputCallback(GLFWwindow* win, int button, int action, int mods);

public:
    const std::string windowName;

private:
    int         winWidth   = 800;
    int         winHeight  = 600;
    GLFWwindow* windowInstance = nullptr;
    MouseDevice mouseDevice;
};

#endif
