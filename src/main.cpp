#include "Viewer.h"
#include "Window.h"
#include <memory>

int main()
{
    auto view = Viewer(std::make_unique<Window>("OpenGL Window"));
    view.Run();
    return 0;
}


