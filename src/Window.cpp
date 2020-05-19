#include <GL/glew.h>
//
#include "Logger.h"
#include "Window.h"
#include <algorithm>
#include <cstdio>
#include <iostream>

static void APIENTRY glDebugOutput(GLenum       source,
                                   GLenum       type,
                                   unsigned int id,
                                   GLenum       severity,
                                   GLsizei      length,
                                   const char*  message,
                                   const void*  userParam)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return; // ignore these non-significant error codes

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

Window::Window(const std::string name)
    : windowName{name}
{
    // initialise glfw and m_windowInstance,
    // create openGL context, initialise any other c++ resources
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_windowInstance = glfwCreateWindow(winWidth, winHeight, name.c_str(), NULL, NULL);

    auto& rLogger = Logger::GetInstance().GetLogger();
    if (m_windowInstance == NULL)
    {
        rLogger.error("Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_windowInstance);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        rLogger.error("Failed to initialize GLEW");
        glfwTerminate();
        return;
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    // glewInit() seems to pollute OpenGL with error codes
    int errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        rLogger.trace("Polluted by glewInit error: {}. Just ignore...", errorCode);
    }
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version  = glGetString(GL_VERSION);  // version as a string
    rLogger.info("Renderer: {}, OpenGL version supported: {}", renderer, version);
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
    CHECK_GL_ERROR_(glViewport(0, 0, static_cast<GLsizei>(winWidth), static_cast<GLsizei>(winHeight)))
    CHECK_GL_ERROR_(glMatrixMode(GL_PROJECTION))
    CHECK_GL_ERROR_(glLoadIdentity())
    CHECK_GL_ERROR_(glMatrixMode(GL_MODELVIEW))
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

void Window::MouseWheelCallback(GLFWwindow* win, double xoffset, double yoffset)
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
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
        {
            if (action == GLFW_PRESS)
            {
                if (!m_MouseLeftBtnDragInfo.bIsDragging)
                {
                    std::cout << "Clicked: (" << m_cursorPos.x() << ", " << m_cursorPos.y() << ")\n";
                    notify(EventType::MOUSE_LEFT_BTN_CLICK, std::make_unique<MouseLeftBtnClickEvent>(m_cursorPos, winWidth, winHeight));
                }
                m_MouseLeftBtnDragInfo.bIsDragging = true;
                m_MouseLeftBtnDragInfo.startPos    = m_cursorPos;
                m_MouseLeftBtnDragInfo.tic         = std::chrono::steady_clock::now();
            }
            else if (action == GLFW_RELEASE)
            {
                m_MouseLeftBtnDragInfo.bIsDragging = false;
                m_MouseLeftBtnDragInfo.toc         = std::chrono::steady_clock::now();
            }
        }
        break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
        {
            if (action == GLFW_PRESS)
            {
                m_MouseMidBtnDragInfo.bIsDragging = true;
                m_MouseMidBtnDragInfo.startPos    = m_cursorPos;
                m_MouseMidBtnDragInfo.tic         = std::chrono::steady_clock::now();
            }
            else if (action == GLFW_RELEASE)
            {
                m_MouseMidBtnDragInfo.bIsDragging = false;
                m_MouseMidBtnDragInfo.toc         = std::chrono::steady_clock::now();
            }
        }
    }
}

void Window::KeyboardDeviceUpdate(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        m_bKeyPressed = true;
    }
    else if (action == GLFW_RELEASE)
    {
        m_bKeyPressed = false;
    }

    m_key = key;

    if (action == GLFW_PRESS)
    {
        switch (m_key)
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(m_windowInstance, true);
                break;
            case GLFW_KEY_W:
                m_bWireFrame = m_bWireFrame ? false : true;
                break;
        }
    }

    notify(EventType::KEY_PRESS, std::make_unique<KeyPressEvent>(key, scancode, action, mods));
}

void Window::MouseWheelUpdate(double xoffset, double yoffset)
{
    notify(EventType::MOUSE_WHEEL, std::make_unique<MouseWheelEvent>(xoffset, yoffset));
}

void Window::CursorPositionUpdate(double xCursorPos, double yCursorPos)
{
    m_cursorPos[0] = static_cast<float>(xCursorPos);
    m_cursorPos[1] = static_cast<float>(yCursorPos);

    m_MouseLeftBtnDragInfo.toc = std::chrono::steady_clock::now();
    if (m_MouseLeftBtnDragInfo.bIsDragging && milliseconds(m_MouseLeftBtnDragInfo.toc - m_MouseLeftBtnDragInfo.tic).count() > 40)
    {
        notify(EventType::MOUSE_LEFT_BTN_DRAG, std::make_unique<MouseLeftBtnDragEvent>(m_MouseLeftBtnDragInfo.startPos, m_cursorPos));
        m_MouseLeftBtnDragInfo.startPos = m_cursorPos;
        m_MouseLeftBtnDragInfo.tic      = m_MouseLeftBtnDragInfo.toc;
    }

    m_MouseMidBtnDragInfo.toc = std::chrono::steady_clock::now();
    if (m_MouseMidBtnDragInfo.bIsDragging && milliseconds(m_MouseMidBtnDragInfo.toc - m_MouseMidBtnDragInfo.tic).count() > 40)
    {
        notify(EventType::MOUSE_MID_BTN_DRAG, std::make_unique<MouseMidBtnDragEvent>(m_MouseMidBtnDragInfo.startPos, m_cursorPos));
        m_MouseMidBtnDragInfo.startPos = m_cursorPos;
        m_MouseMidBtnDragInfo.tic      = m_MouseMidBtnDragInfo.toc;
    }
}

void Window::attach(std::shared_ptr<IObserver> spObserver)
{
    m_observerList.push_back(spObserver);
}

void Window::detach(std::shared_ptr<IObserver> spObserver)
{
    std::remove_if(std::begin(m_observerList), std::end(m_observerList), [](std::weak_ptr<IObserver> o) {
        return o.expired();
    });
}

void Window::detach(std::list<std::weak_ptr<IObserver>>::const_iterator iter)
{
    m_observerList.erase(iter);
}

void Window::notify(const EventType& eventType, std::unique_ptr<IEvent> spEvent)
{
    for (auto it = std::cbegin(m_observerList); it != std::cend(m_observerList); ++it)
    {
        if (auto observer = it->lock())
        {
            observer->onNotify(eventType, spEvent.get());
        }
        else
        {
        }
    }
}
