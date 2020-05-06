#include "Object3D.h"
#include "File.h"
#include "ObjFileParser.h"
#include <iostream>
#include <GL/glew.h>

Object3D::Object3D(const std::string& pathToModel)
    : path{pathToModel}
{
    spMesh3D = ObjFileParser().Parse(std::make_unique<File>(path));
    if (spMesh3D != nullptr)
    {
        spOGLDataObject->InitializeVertexBuffer(*spMesh3D);

        if (spMesh3D->HasNormals())
        {
            spOGLDataObject->InitializeNormalBuffer(*spMesh3D);
        }
        /* else */
        /* { */

        /* } */

        /* if (spObjectMesh3D->HasUVCoordinates()) */
        /* { */
        /*     spOGLDataObject->InitializeTextureUVBuffer(spObjectMesh3D->uvCoordinates); */
        /* } */

        spOGLDataObject->InitializeColorBuffer(vertexColor);
    }
}

void Object3D::SetColor(const Color& color)
{
    vertexColor = color;
    spOGLDataObject->InitializeColorBuffer(vertexColor);
}

void Object3D::Draw(void)
{
    spOGLDataObject->DrawObject(GL_TRIANGLES);
}


void Object3D::onNotify(const EventType& eventType, std::unique_ptr<IEvent> spEventData)
{
	MouseDragEvent* pMouseDragEvent = dynamic_cast<MouseDragEvent*>(spEventData.get());
	if(pMouseDragEvent != nullptr)
	{
		std::cout << pMouseDragEvent->m_endCoordinates - pMouseDragEvent->m_startCoordinates << std::endl;
	}
	else
	{
		std::cout << "Cast fehlgeschlagen" << std::endl;
	}
}
