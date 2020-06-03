#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Camera.h"
#include "Color.h"
#include "Mesh3D.h"
#include "Shader.h"
#include <memory>
#include <string>

class IRenderable
{
public:
    virtual ~IRenderable()                                                                                                  = default;
    virtual void Init(std::shared_ptr<Mesh3D> spMesh3D, std::shared_ptr<Camera> spCamera, std::shared_ptr<Shader> spShader) = 0;
    virtual void Render()                                                                                                   = 0;
};
#endif
