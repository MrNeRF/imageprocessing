#include "AlgoVertexNormals.h"
#include "VertexNormalAttribute.h"
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <numeric>

bool AlgoVertexNormals::Compute()
{
    bool bResult = false;
    switch (m_type)
    {
        case NormalType::SMOOTH:
            bResult = ComputeSmoothNormals();
            break;
        case NormalType::NONSMOOTH:
            bResult = ComputeNonSmoothNormals();
            break;
    }
    return bResult;
}

bool AlgoVertexNormals::ComputeSmoothNormals()
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

bool AlgoVertexNormals::ComputeNonSmoothNormals()
{
    std::vector<Eigen::Vector3f> faceNormals;
    faceNormals.resize(m_rMesh.GetNumberOfFaces());
    std::vector<uint32_t> vertexNormalIndices;
    vertexNormalIndices.resize(faceNormals.size() * 3);

    const std::vector<uint32_t> meshIndices = m_rMesh.GetIndices();
    uint32_t                    idx         = 0;
    for (uint32_t i = 0; i < meshIndices.size(); i += 3)
    {
        auto vecA   = m_rMesh.GetVertex(meshIndices[i]);
        auto vecB   = m_rMesh.GetVertex(meshIndices[i + 1]);
        auto vecC   = m_rMesh.GetVertex(meshIndices[i + 2]);
        auto normal = ((vecB - vecA).cross(vecC - vecA));
        normal.normalize();
        faceNormals[idx]           = normal;
        vertexNormalIndices[i]     = idx;
        vertexNormalIndices[i + 1] = idx;
        vertexNormalIndices[i + 2] = idx++;
        /* std::cout << "Normal " << i << ": " << normal.x() << ", " << normal.y() << ", " << normal.z() << "\n"; */
    }

    auto& rNormals = dynamic_cast<VertexNormalAttribute&>(m_rMesh.AddVertexAttribute(Mesh3D::EVertexAttribute::Normal));
    rNormals.SetVertexNormals(faceNormals, vertexNormalIndices);
    return true;
}
