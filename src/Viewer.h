#ifndef VIEWER_H
#define VIEWER_H

#include <memory>

class Window;

class Viewer
{
public:
    Viewer(std::unique_ptr<Window> window);
    ~Viewer(void) = default;

    void Run(void);

private:
    std::unique_ptr<Window> m_window;
};

#endif
