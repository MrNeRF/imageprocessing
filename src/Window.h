#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include "GLFW/glfw3.h"
#include <Eigen/Dense>
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

        double clickedPosX = 0.;
        double clickedPosY = 0.;
        double cursorPosX  = 0.;
        double cursorPosY  = 0.;
        bool   isPressed   = false;
        bool   isReleased  = true;

        MouseButton currentButton = MouseButton::NONE;
        MouseAction currentAction = MouseAction::RELEASE;
    };

public:
    Window(const std::string name);
    ~Window(void);

    void ViewPortResized(int width, int height);
    void MouseDeviceUpdate(GLFWwindow* win, int button, int action, int mods);
    void            UpdateCursorPosition(double xCursorPos, double yCursorPos);
    GLFWwindow* GetGLFWWindow(void) { return windowInstance; }
    Eigen::Vector2f GetCursorPosition(void) { return Eigen::Vector2f(static_cast<float>(mouseDevice.cursorPosX), static_cast<float>(mouseDevice.cursorPosY)); }

    // Callbacks
    static void WindowResizeCallback(GLFWwindow* win, int h, int w);
    static void MouseInputCallback(GLFWwindow* win, int button, int action, int mods);
    static void CursorPositionCallback(GLFWwindow* win, double xCursorPos, double yCursorPos);

public:
    const std::string windowName;

private:
    int         winWidth   = 800;
    int         winHeight  = 600;
    GLFWwindow* windowInstance = nullptr;
    MouseDevice mouseDevice;
};

#endif
