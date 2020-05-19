#ifndef ALGO_VERTEX_NORMALS
#define ALGO_VERTEX_NORMALS

#include "Algorithm.h"
#include "Mesh3D.h"

class AlgoVertexNormals : public Algorithm
{
public:
    AlgoVertexNormals(Mesh3D& rMesh)
        : m_rMesh{rMesh} {};
    [[nodiscard]] bool Compute(void) override;

private:
    Mesh3D& m_rMesh;
};
#endif
