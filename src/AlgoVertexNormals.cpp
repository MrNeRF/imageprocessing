#include "AlgoVertexNormals.h"
#include "VertexNormalAttribute.h"
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <numeric>

bool AlgoVertexNormals::Compute()
{
    std::vector<Eigen::Vector3f> faceNormals;
    faceNormals.resize(m_rMesh.GetNumberOfFaces());
    std::vector<Eigen::Vector3f> vertexNormals;
    vertexNormals.resize(m_rMesh.GetNumberOfVertice());

    int i = 0;
    for (const auto& faceID : faceIterator(m_rMesh))
    {
        auto [vecA, vecB, vecC] = m_rMesh.GetFaceVertices(faceID);
        auto normal             = ((vecB - vecA).cross(vecC - vecA));
        normal.normalize();
        faceNormals[faceID] = normal;
        /* std::cout << "Normal " << i << ": " << normal.x() << ", " << normal.y() << ", " << normal.z() << "\n"; */
        ++i;
    }

    i = 0;
    for (const auto& vertexID : vertexIterator(m_rMesh))
    {
        std::vector<Eigen::Vector3f> normals;
        for (const auto& faceID : oneRingFaceIterator(m_rMesh, vertexID))
        {
            normals.push_back(faceNormals[faceID]);
        }

        auto normal = std::accumulate(normals.begin(), normals.end(), Eigen::Vector3f(0.f, 0.f, 0.f));
        normal.normalize();
        vertexNormals[vertexID] = normal;
        /* std::cout << "Normal " << i << ": " << normal.x() << ", " << normal.y() << ", " << normal.z() << "\n"; */
        ++i;
    }

    auto& rNormals = dynamic_cast<VertexNormalAttribute&>(m_rMesh.AddVertexAttribute(Mesh3D::EVertexAttribute::Normal));
    rNormals.SetVertexNormals(vertexNormals, m_rMesh.GetIndices());
    return true;
}
