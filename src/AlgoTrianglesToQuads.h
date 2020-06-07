#ifndef ALGO_TRIANGLES_TO_QUADS
#define ALGO_TRIANGLES_TO_QUADS

#include "Algorithm.h"
#include "Mesh3D.h"

class AlgoTrianglesToQuads : public Algorithm
{
public:
    AlgoTrianglesToQuads(Mesh3D* pMesh)
        : m_pMesh{pMesh} {};
    bool Compute() override;

private:
    Mesh3D* m_pMesh;
};

#endif
