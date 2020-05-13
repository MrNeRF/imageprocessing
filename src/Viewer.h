#ifndef VIEWER_H
#define VIEWER_H

#include <memory>
#include <string>
#include <vector>
#include "IRenderable.h"

class Window;

class Viewer
{
public:
	Window* Init(const std::string& name);
    void AddRenderObject(std::shared_ptr<IRenderable> spRenderObject);
    std::shared_ptr<IRenderable> GetLastRenderObject();
    void Run(void);

private:
    std::unique_ptr<Window> m_spWindow;
	std::vector<std::shared_ptr<IRenderable>> m_renderObjects;
};

#endif
