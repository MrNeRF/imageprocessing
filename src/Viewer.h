#ifndef VIEWER_H
#define VIEWER_H

#include <memory>
#include <string>

class Window;

class Viewer
{
public:
    Viewer(std::unique_ptr<Window> window);
    ~Viewer(void);

    void Run(void);

private:
    void registerCallbacks(void);

private:
    std::unique_ptr<Window> m_window;
    unsigned int VBO, VAO, EBO;
};

#endif
