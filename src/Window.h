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

    struct KeyboardDevice
    {
        enum class KeyAction
        {
            RELEASE,
            PRESS,
            REPEAT
        };

        KeyAction currentAction = KeyAction::RELEASE;
        int  key        = -1;
        bool isPressed  = false;
        bool isReleased = true;
    };

public:
    Window(const std::string name);
    ~Window(void);

    void ViewPortResized(int width, int height);
    void MouseDeviceUpdate(GLFWwindow* win, int button, int action, int mods);
    void UpdateCursorPosition(double xCursorPos, double yCursorPos);
    void KeyboardDeviceUpdate(int key, int scancode, int action, int mods);

    GLFWwindow*     GetGLFWWindow(void) { return windowInstance; }
    Eigen::Vector2f GetCursorPosition(void) { return Eigen::Vector2f(static_cast<float>(mouseDevice.cursorPosX), static_cast<float>(mouseDevice.cursorPosY)); }

    // Callbacks
    static void WindowResizeCallback(GLFWwindow* win, int h, int w);
    static void MouseInputCallback(GLFWwindow* win, int button, int action, int mods);
    static void CursorPositionCallback(GLFWwindow* win, double xCursorPos, double yCursorPos);
    static void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods);

public:
    int   winHeight   = 800;
    int   winWidth    = 600;
    float aspectRatio = winWidth / winHeight;

    MouseDevice    mouseDevice;
    KeyboardDevice keyboardDevice;

    const std::string windowName;

private:
    GLFWwindow* windowInstance = nullptr;
};

#endif
