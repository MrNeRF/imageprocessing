#include "Logger.h"
#include "Viewer.h"
#include "Window.h"
#include <memory>
#include <spdlog/spdlog.h>

int main()
{
    auto& logger = Logger::GetInstance().GetLogger();
    logger.info("main()");

    auto view = Viewer(std::make_unique<Window>("OpenGL Window"));
    view.Run();
    return 0;
}


