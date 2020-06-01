#include "BoundingVolume.h"
#include "AlgoVertexNormals.h"
#include "Quader.h"
#include "Sphere.h"
#include "VertexColorAttribute.h"
#include "VertexNormalAttribute.h"
#include <deque>
#include <iterator>
#include <limits>

// Welzl Miniball Algorithm
static Sphere recursiveMB(std::deque<Eigen::Vector3f>& points, std::vector<Eigen::Vector3f> bound)
{
    Sphere mb;
    if (points.empty() || bound.size() == 4)
    {
        switch (bound.size())
        {
            case 0:
                mb = Sphere();
                break;
            case 1:
                mb = Sphere(bound[0]);
                break;
            case 2:
                mb = Sphere(bound[0], bound[1]);
                break;
            case 3:
                mb = Sphere(bound[0], bound[1], bound[2]);
                break;
            case 4:
                mb = Sphere(bound[0], bound[1], bound[2], bound[3]);
                break;
        }
    }
    else
    {
        Eigen::Vector3f p = points.back();
        points.pop_back();
        mb                       = recursiveMB(points, bound);
        const Eigen::Vector3f sc = mb.GetCenter();
        const float           r2 = mb.GetRadius() * mb.GetRadius();
        if ((sc - p).squaredNorm() > r2)
        {
            bound.push_back(p);
            mb = recursiveMB(points, bound);
            // move to front
            points.push_back(p);
        }
    }

    return mb;
}

BoundingVolume::BoundingVolume(const BoundingVolume::EBoundingVolume& type, const Mesh3D& rMeshToBound)
    : m_bvType{type}
{
    m_spShader = std::make_shared<Shader>("Line");
    m_spShader->InitShaders("../Shaders/color.vs", "../Shaders/color.fs");
    m_spOGLDataObject = std::make_shared<OpenGL3DDataObject>();

    switch (m_bvType)
    {
        case EBoundingVolume::Sphere:
        {
            createBoundingSphere(rMeshToBound);
        }
        break;
        case EBoundingVolume::Cube:
        {
            createBoundingCube(rMeshToBound);
        }
        break;
    }
}

void BoundingVolume::createBoundingCube(const Mesh3D& rMeshToBound)
{
    Eigen::Vector3f max = Eigen::Vector3f(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
    Eigen::Vector3f min = Eigen::Vector3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

    for (const auto& vertex : vertexIterator(rMeshToBound))
    {
        const auto& v = rMeshToBound.GetVertex(vertex);
        max[0]        = std::max(max[0], v[0]);
        max[1]        = std::max(max[1], v[1]);
        max[2]        = std::max(max[2], v[2]);

        min[0] = std::min(min[0], v[0]);
        min[1] = std::min(min[1], v[1]);
        min[2] = std::min(min[2], v[2]);
    }

    m_spBoundingVolume = std::make_unique<Quader>(min, max);
    m_spBVMesh         = m_spBoundingVolume->CreateMesh();

    VertexColorAttribute&        rColor = dynamic_cast<VertexColorAttribute&>(m_spBVMesh->AddVertexAttribute(Mesh3D::EVertexAttribute::Color));
    std::vector<Eigen::Vector3f> colorData(m_spBVMesh->GetNumberOfVertice(), Color::GetColor(Color::EColor::YELLOW));
    rColor.SetVertexColor(colorData, m_spBVMesh->GetIndices());
    auto pNormal = dynamic_cast<VertexNormalAttribute*>(m_spBVMesh->GetVertexAttribute(Mesh3D::EVertexAttribute::Normal));
    if (pNormal == nullptr)
    {
        AlgoVertexNormals algo(*m_spBVMesh, AlgoVertexNormals::NormalType::SMOOTH);
        ASSERT(algo.Compute() == true);
    }

    m_spOGLDataObject->InitializeBuffer(*m_spBVMesh);
}
void BoundingVolume::createBoundingSphere(const Mesh3D& rMeshToBound)
{
    std::vector<Eigen::Vector3f> vertices = rMeshToBound.GetVertices();
    std::deque<Eigen::Vector3f>  input;
    for (const auto& rVertex : vertices)
    {
        input.push_back(rVertex);
    }
    std::vector<Eigen::Vector3f> bound;

    Sphere sphere = recursiveMB(input, bound);
    fmt::print("Center: ({}, {}, {}); Radius: {}\n", sphere.GetCenter().x(), sphere.GetCenter().x(), sphere.GetCenter().x(), sphere.GetRadius());
    m_spBoundingVolume = std::make_unique<Sphere>(sphere);
    m_spBVMesh         = m_spBoundingVolume->CreateMesh();

    VertexColorAttribute&        rColor = dynamic_cast<VertexColorAttribute&>(m_spBVMesh->AddVertexAttribute(Mesh3D::EVertexAttribute::Color));
    std::vector<Eigen::Vector3f> colorData(m_spBVMesh->GetNumberOfVertice(), Color::GetColor(Color::EColor::YELLOW));
    rColor.SetVertexColor(colorData, m_spBVMesh->GetIndices());

    auto pNormal = dynamic_cast<VertexNormalAttribute*>(m_spBVMesh->GetVertexAttribute(Mesh3D::EVertexAttribute::Normal));
    if (pNormal == nullptr)
    {
        AlgoVertexNormals algo(*m_spBVMesh, AlgoVertexNormals::NormalType::SMOOTH);
        ASSERT(algo.Compute() == true);
    }

    m_spOGLDataObject->InitializeBuffer(*m_spBVMesh);
}

bool BoundingVolume::IsBHHit(const Ray& rRay, Eigen::Vector3f translate)
{
    switch (m_bvType)
    {
        case EBoundingVolume::Cube:
        {
            Quader* pCube         = static_cast<Quader*>(m_spBoundingVolume.get());
            auto [bHit, hitPoint] = pCube->IsHit(rRay, translate);
            return bHit;
        }
        break;
        case EBoundingVolume::Sphere:
        {
            Sphere* pSphere       = static_cast<Sphere*>(m_spBoundingVolume.get());
            auto [bHit, hitPoint] = pSphere->IsHit(rRay, translate);
            if (bHit)
            {
                std::cout << "HIT !!!" << std::endl;
            }
            return bHit;
        }
    }

    return false;
}

void BoundingVolume::Draw(const Eigen::Matrix4f& model, const Camera& rCam)
{
    m_spShader->UseShader();
    m_spShader->SetTransformationMatrix("model", model);
    m_spShader->SetTransformationMatrix("view", rCam.GetLookAt());
    m_spShader->SetTransformationMatrix("projection", rCam.GetPerspectiveProjection());

    GLint polygonMode;
    glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
    bool bIsModeSwitched = false;
    if (polygonMode == GL_FILL)
    {
        bIsModeSwitched = true;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    m_spOGLDataObject->DrawObject(GL_TRIANGLES);
    if (bIsModeSwitched)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
