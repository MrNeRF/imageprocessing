#include "Object3D.h"
#include "Color.h"
#include "File.h"
#include "ObjFileParser.h"
#include <GL/glew.h>

Object3D::Object3D(const std::string& pathToModel)
    : path{pathToModel}
{
    spObjectMesh3D = ObjFileParser().Parse(std::make_unique<File>(path));
    if (spObjectMesh3D != nullptr)
    {
        spOGLDataObject->InitializeVertexBuffer(spObjectMesh3D->vertices, spObjectMesh3D->indices);

        if (spObjectMesh3D->HasNormals())
        {
            spOGLDataObject->InitializeNormalBuffer(spObjectMesh3D->normals);
        }

        if (spObjectMesh3D->HasUVCoordinates())
        {
            spOGLDataObject->InitializeTextureUVBuffer(spObjectMesh3D->uvCoordinates);
        }

        spOGLDataObject->InitializeColorBuffer(Color(0.8f, 0.3f, 0.9));
    }
}

void Object3D::Draw(void)
{
    spOGLDataObject->DrawObject(GL_TRIANGLES);
}

