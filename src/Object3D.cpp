#include "Object3D.h"
#include "File.h"
#include "Macros.h"
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


void Object3D::onNotify(const EventType& eventType, IEvent* pEventData)
{
	if (eventType == EventType::MOUSEDRAG)
	{
		MouseDragEvent* pMouseDragEvent = dynamic_cast<MouseDragEvent*>(pEventData);
		if(pMouseDragEvent != nullptr)
		{
			Eigen::Vector2f difference = (pMouseDragEvent->m_endCoordinates - pMouseDragEvent->m_startCoordinates).normalized();
			/* UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(5.f), */ 
			/* 			Eigen::Vector3f(-difference[1], difference[0], 0.f))); */
			UpdateOrientation(Eigen::AngleAxisf(MathHelper::degreeToRadians(10.f), 
						Eigen::Vector3f(difference.y(), difference.x(), 0.f)));
		}
		else
		{
			// We should not get here -> BUG
			ASSERT(0);
		}
	}
}
