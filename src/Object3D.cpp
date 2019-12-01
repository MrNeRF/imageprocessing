#include "Object3D.h"
#include "File.h"
#include "ObjFileParser.h"
#include <GL/glew.h>

Object3D::Object3D(const std::string& pathToModel)
    : path{pathToModel}
{
    spObjectMesh3D = ObjFileParser().Parse(std::make_unique<File>(path));
    spOGLDataObject->InitializeVertexBuffer(spObjectMesh3D->vertices, spObjectMesh3D->indices);
}

void Object3D::Draw(void)
{
    spOGLDataObject->DrawObject(GL_TRIANGLES);
}

