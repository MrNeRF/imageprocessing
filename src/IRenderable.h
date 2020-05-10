#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Camera.h"
#include "Color.h"
#include "Shader.h"
#include <memory>
#include <string>

class IRenderable
{
public:
    virtual ~IRenderable()                                                                                                = default;
    virtual void Init(const std::string& pathToModel, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader) = 0;
    virtual void SetColor(const Color& color) = 0;
    virtual void Render()                     = 0;
};
#endif
