#ifndef ALGO_VERTEX_NORMALS
#define ALGO_VERTEX_NORMALS

#include "Algorithm.h"
#include "Mesh3D.h"

class AlgoVertexNormals : public Algorithm
{
public:
    enum class NormalType
    {
        SMOOTH,
        NONSMOOTH
    };

public:
    AlgoVertexNormals(Mesh3D& rMesh, NormalType type)
        : m_rMesh{rMesh}
        , m_type{type} {};
    [[nodiscard]] bool Compute(void) override;
    [[nodiscard]] bool ComputeSmoothNormals(void);
    [[nodiscard]] bool ComputeNonSmoothNormals(void);

private:
    Mesh3D&          m_rMesh;
    const NormalType m_type;
};
#endif
