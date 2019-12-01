#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Mesh3D.h"
#include "OpenGL3DDataObject.h"
#include <memory>
#include <string>

class Object3D
{
public:
    Object3D(const std::string& pathToModel);
    void Draw(void);

private:
    std::string path;
    std::unique_ptr<Mesh3D> spObjectMesh3D;
    std::unique_ptr<OpenGL3DDataObject> spOGLDataObject = std::make_unique<OpenGL3DDataObject>();
};

#endif
