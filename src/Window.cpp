#include <GL/glew.h>
#include "Window.h"
#include <algorithm>
#include <cstdio>
#include <iostream>

Window::Window(const std::string name)
    : windowName{name}
{
    // initialise glfw and m_windowInstance,
    // create openGL context, initialise any other c++ resources
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_windowInstance = glfwCreateWindow(winHeight, winWidth, "Window", NULL, NULL);

    if (m_windowInstance == NULL)
    {
        std::puts("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_windowInstance);
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
    glfwSetWindowUserPointer(m_windowInstance, this);

    // Register Callbacks
    glfwSetFramebufferSizeCallback(m_windowInstance, WindowResizeCallback);
    glfwSetMouseButtonCallback(m_windowInstance, MouseInputCallback);
    glfwSetScrollCallback(m_windowInstance, MouseWheelCallback);
    glfwSetCursorPosCallback(m_windowInstance, CursorPositionCallback);
    glfwSetKeyCallback(m_windowInstance, KeyboardCallback);
}

Window::~Window(void)
{
    glfwTerminate();
}

void Window::ViewPortUpdate(int width, int height)
{
    winWidth  = width;
    winHeight = height;
    glViewport(0, 0, static_cast<GLsizei>(winWidth), static_cast<GLsizei>(winHeight));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    aspectRatio = static_cast<float>(winWidth) / static_cast<float>(winHeight);
}

void Window::WindowResizeCallback(GLFWwindow* win, int h, int w)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->ViewPortUpdate(w, h);
}

void Window::MouseInputCallback(GLFWwindow* win, int button, int action, int mods)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->MouseDeviceUpdate(win, button, action, mods);
}

void Window::MouseWheelCallback(GLFWwindow *win, double xoffset, double yoffset)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->MouseWheelUpdate(xoffset, yoffset);
}

void Window::CursorPositionCallback(GLFWwindow* win, double xCursorPos, double yCursorPos)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->CursorPositionUpdate(xCursorPos, yCursorPos);
}

void Window::KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->KeyboardDeviceUpdate(key, scancode, action, mods);
}

void Window::MouseDeviceUpdate(GLFWwindow* win, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			m_MouseDragInfo.bIsDragging = true;
			m_MouseDragInfo.startPos = m_cursorPos; 
			m_MouseDragInfo.tic = std::chrono::steady_clock::now();
		}
		else if (action == GLFW_RELEASE)
		{
			m_MouseDragInfo.bIsDragging = false;
			m_MouseDragInfo.toc = std::chrono::steady_clock::now();
		}
	}

}

void Window::KeyboardDeviceUpdate(int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		m_bKeyPressed = true;	
	}
	else if (action == GLFW_RELEASE)
	{

		m_bKeyPressed = false;	
	}

	m_key = key;
}

void Window::MouseWheelUpdate(double xoffset, double yoffset)
{
	notify(EventType::MOUSEWHEEL, std::make_unique<MouseWheelEvent>(xoffset, yoffset));
}

void Window::CursorPositionUpdate(double xCursorPos, double yCursorPos)
{
	m_cursorPos[0] = static_cast<float>(xCursorPos);
	m_cursorPos[1]	= static_cast<float>(yCursorPos);

	m_MouseDragInfo.toc = std::chrono::steady_clock::now();
	if(m_MouseDragInfo.bIsDragging && milliseconds(m_MouseDragInfo.toc - m_MouseDragInfo.tic).count() > 40)
	{
		notify(EventType::MOUSEDRAG, std::make_unique<MouseDragEvent>(m_MouseDragInfo.startPos, m_cursorPos));
		m_MouseDragInfo.startPos = m_cursorPos;
		m_MouseDragInfo.tic = m_MouseDragInfo.toc;
	}

}

void Window::attach(std::shared_ptr<IObserver> spObserver)
{
	m_observerList.push_back(spObserver);
}

void Window::detach(std::shared_ptr<IObserver> spObserver)
{
	std::remove_if(std::begin(m_observerList), std::end(m_observerList), [](std::weak_ptr<IObserver> o)
			{
				return o.expired();
			});
}

void Window::detach(std::list<std::weak_ptr<IObserver>>::const_iterator iter)
{
	m_observerList.erase(iter);
}

void Window::notify(const EventType &eventType, std::unique_ptr<IEvent> spEvent)
{
	for (auto it = std::cbegin(m_observerList); it != std::cend(m_observerList); ++it)
	{
		if(auto observer = it->lock())
		{
			observer->onNotify(eventType, spEvent.get());
		}
		else
		{

		}
	}
}
