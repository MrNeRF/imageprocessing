#ifndef MOUSE_DEVICE_H
#define MOUSE_DEVICE_H

#include <stdint.h>

class MouseDevice
{
public:
    enum class MouseButton
    {
        None,
        LeftMouseButton,
        MiddleMouseButton,
        RightMouseButton
    };

public:
    void SetXPos(uint32_t x) { m_xPos = x; };
    void SetYPos(uint32_t y) { m_xPos = y; };

    uint32_t GetXPos(void) { return m_xPos; };
    uint32_t GetYPos(void) { return m_yPos; };

    bool IsPressed(void) { return m_pressed != MouseButton::None; };
    bool IsRelease(void) { return m_isReleased; };

    void        SetMouseButtonPressed(MouseButton pressedButton) { m_pressed = pressedButton; };
    MouseButton GetPressedMouseButton(void) { return m_pressed; };

private:
    uint32_t m_xPos     = 0;
    uint32_t m_yPos     = 0;
    bool     m_isPressed  = false;
    bool     m_isReleased = true;

    MouseButton m_pressed = MouseButton::None;
};

#endif 
