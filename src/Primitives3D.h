#ifndef PRIMITIVES_3D_H
#define PRIMITIVES_3D_H

#include "Mesh3D.h"
#include <memory>

class Primitives3D
{
	public:
		virtual ~Primitives3D() = default;
		virtual std::shared_ptr<Mesh3D> GetMesh() = 0;
};

#endif

