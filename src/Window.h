#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include "GLFW/glfw3.h"
#include <Eigen/Dense>
#include "IObservable.h"
#include "IEvent.h"
#include <chrono>
#include <ratio>
#include <cstdint>
#include <string>
#include <list>

class Window : public IObservable
{
	using seconds = std::chrono::duration<double>;
	using milliseconds = std::chrono::duration<double, std::ratio_multiply<seconds::period, std::milli>>;
	struct MouseLeftBtnDragInfo
	{
		Eigen::Vector2f startPos;
		Eigen::Vector2f endPos;
		bool bIsDragging = false;
		decltype(std::chrono::steady_clock::now()) tic;
		decltype(std::chrono::steady_clock::now()) toc;
	};

	struct MouseMidBtnDragInfo
	{
		Eigen::Vector2f startPos;
		Eigen::Vector2f endPos;
		bool bIsDragging = false;
		decltype(std::chrono::steady_clock::now()) tic;
		decltype(std::chrono::steady_clock::now()) toc;
	};

public:
    Window(const std::string name);
    ~Window(void);

    // Callbacks
    static void WindowResizeCallback(GLFWwindow* win, int h, int w);
    static void MouseInputCallback(GLFWwindow* win, int button, int action, int mods);
    static void MouseWheelCallback(GLFWwindow* win, double xoffset, double yoffset);
    static void CursorPositionCallback(GLFWwindow* win, double xCursorPos, double yCursorPos);
    static void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods);

    GLFWwindow*     GetGLFWWindow(void) { return m_windowInstance; }

    // Associated class methods invoked by registered callbacks.
    void ViewPortUpdate(int width, int height);
    void MouseDeviceUpdate(GLFWwindow* win, int button, int action, int mods);
    void MouseWheelUpdate(double xoffset, double yoffset);
    void CursorPositionUpdate(double xCursorPos, double yCursorPos);
    void KeyboardDeviceUpdate(int key, int scancode, int action, int mods);
	
	// Observable overrides
	void attach(std::shared_ptr<IObserver> spObserver) override;	
	void detach(std::shared_ptr<IObserver> spObserver) override;
	void detach(std::list<std::weak_ptr<IObserver>>::const_iterator iter);
    void notify(const EventType& eventType, std::unique_ptr<IEvent> spEvent) override;


public:
    int   winHeight   = 800;
    int   winWidth    = 600;
    float aspectRatio = winWidth / winHeight;
    const std::string windowName;
    bool m_bKeyPressed = false;
    int m_key = 0;;
	bool m_bWireFrame = false;

private:
	MouseLeftBtnDragInfo m_MouseLeftBtnDragInfo;
	MouseMidBtnDragInfo m_MouseMidBtnDragInfo;
    GLFWwindow* m_windowInstance = nullptr;
	Eigen::Vector2f m_cursorPos;
	std::list<std::weak_ptr<IObserver>> m_observerList;
};

#endif
