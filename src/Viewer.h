#ifndef VIEWER_H
#define VIEWER_H

#include <string>

class GLFWwindow;

class Viewer
{
public:
    Viewer(const std::string& windowName);
    ~Viewer(void);

    void Run(void);

private:
    void registerCallbacks(void);

private:
    GLFWwindow*  m_window;
    unsigned int VBO, VAO, EBO;
};

#endif
