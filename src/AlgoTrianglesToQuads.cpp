#include "AlgoTrianglesToQuads.h"
#include "AlgoVertexNormals.h"
#include "Color.h"
#include "FaceColorAttribute.h"
#include "HSLColor.h"
#include "VertexNormalAttribute.h"
#include <Eigen/Dense>
#include <limits>
#include <map>
#include <random>
#include <set>
#include <vector>

static std::vector<Eigen::Vector3f> generators{
    Eigen::Vector3f(-1.f, 1.f, -1.f).normalized(),
    Eigen::Vector3f(1.f, 1.f, -1.f).normalized(),
    Eigen::Vector3f(1.f, -1.f, -1.f).normalized(),
    Eigen::Vector3f(-1.f, -1.f, -1.f).normalized(),
    Eigen::Vector3f(-1.f, 1.f, 1.f).normalized(),
    Eigen::Vector3f(1.f, 1.f, 1.f).normalized(),
    Eigen::Vector3f(1.f, -1.f, 1.f).normalized(),
    Eigen::Vector3f(-1.f, -1.f, 1.f).normalized(),
    Eigen::Vector3f(0.f, 1.f, 0.f).normalized(),
    Eigen::Vector3f(1.f, 0.f, 0.f).normalized(),
    Eigen::Vector3f(0.f, -1.f, 0.f).normalized(),
    Eigen::Vector3f(-1.f, 0.f, 0.f).normalized(),
    Eigen::Vector3f(0.f, 0.f, 1.f).normalized(),
    Eigen::Vector3f(0.f, 0.f, -1.f).normalized(),
    Eigen::Vector3f(1.f, 1.f, 0.f).normalized(),
    Eigen::Vector3f(-1.f, 1.f, 0.f).normalized(),
    Eigen::Vector3f(1.f, -1.f, 0.f).normalized(),
    Eigen::Vector3f(0.f, 1.f, 1.f).normalized(),
    Eigen::Vector3f(0.f, -1.f, 1.f).normalized(),
    Eigen::Vector3f(0.f, 1.f, -1.f).normalized(),
    Eigen::Vector3f(0.f, -1.f, -1.f).normalized(),
    Eigen::Vector3f(-1.f, 0.f, -1.f).normalized(),
    Eigen::Vector3f(1.f, 0.f, -1.f).normalized(),
    Eigen::Vector3f(-1.f, 0.f, 1.f).normalized(),
    Eigen::Vector3f(1.f, 0.f, 1.f).normalized(),
};

bool AlgoTrianglesToQuads::Compute()
{
    for (uint32_t k = 0; k < generators.size(); ++k)
    {
        for (uint32_t l = 0; l < generators.size(); ++l)
        {
            if (k != l)
            {
                ASSERT(generators[l] != generators[k])
            }
        }
    }
    AlgoVertexNormals algo(*m_pMesh, AlgoVertexNormals::NormalType::NONSMOOTH);
    ASSERT(algo.Compute());
    std::vector<uint32_t>  clusters(generators.size(), 0);
    VertexNormalAttribute& normals = dynamic_cast<VertexNormalAttribute&>(*m_pMesh->GetVertexAttribute(Mesh3D::EVertexAttribute::Normal));

    std::map<uint32_t, uint32_t> faceRegionMapping;
    bool                         bUpdated = true;
    while (bUpdated)
    {
        bUpdated = false;
        for (uint32_t i = 0; i < m_pMesh->GetNumberOfFaces() * 3; i += 3)
        {
            const Eigen::Vector3f n = normals[i];

            float    maxNorm = std::numeric_limits<float>::max();
            uint32_t i_star  = 0;
            for (uint32_t j = 0; j < generators.size(); ++j)
            {
                if ((n - generators[j]).norm() < maxNorm)
                {
                    maxNorm = (n - generators[j]).norm();
                    i_star  = j;
                }
            }

            uint32_t faceID = i / 3;
            if (faceRegionMapping.find(faceID) == faceRegionMapping.end())
            {
                // Non existent element
                faceRegionMapping[faceID] = i_star;
                clusters[i_star] += 1;
                bUpdated = true;
            }
            else
            {
                uint32_t u = faceRegionMapping[faceID];
                if (u == i_star)
                {
                    continue;
                }
                faceRegionMapping[faceID] = i_star;
                generators[i_star]        = (clusters[i_star] * generators[i_star] + n) / (clusters[i_star] + 1);
                generators[u]             = (clusters[u] * generators[u] - n) / (clusters[u] - 1);
                clusters[i_star] += 1;
                clusters[u] -= 1;
                bUpdated = true;
            }
        }
    }

    FaceColorAttribute&          rColor = dynamic_cast<FaceColorAttribute&>(m_pMesh->AddFaceAttribute(Mesh3D::EFaceAttribute::Color));
    std::vector<Eigen::Vector3f> hColorAttribute(m_pMesh->GetNumberOfFaces(), Color::GetColor(Color::EColor::GREEN));

    std::vector<Color>               regionColorMapping;
    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    uint32_t                         step = static_cast<uint32_t>(std::floor(360.f / static_cast<float>(clusters.size())));
    for (uint32_t i = 0; i < 361; i += step)
    {
        regionColorMapping.push_back(HSLColor::ConvertHueToRGB(static_cast<float>(i), 90.f + dis(gen) * 10.f, 50.f + dis(gen) * 10.f));
    }

    for (uint32_t n = 0; n < regionColorMapping.size(); ++n)
    {
        if (n != 0)
        {
            regionColorMapping[n] = Color(0.f, 0.f, 0.f);
        }
    }

    for (const auto& [face, region] : faceRegionMapping)
    {
        hColorAttribute[face] = regionColorMapping[region].GetColor();
    }
    rColor.SetFaceColor(hColorAttribute);
    return true;
}
