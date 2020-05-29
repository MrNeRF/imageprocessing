#include "Sphere.h"
#include "Macros.h"
#include "VertexNormalAttribute.h"

void Sphere::SetSphereResolution(uint32_t stackCount, uint32_t sectorCount)
{
    m_stackCount  = stackCount;
    m_sectorCount = sectorCount;
}
Sphere::Sphere(const Eigen::Vector3f& p0)
{
    m_creationConstructor = 1;
    m_radius              = std::numeric_limits<float>::epsilon();
    m_center              = p0;
}
Sphere::Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1)
{
    m_creationConstructor = 2;

    m_center = (p0 + p1) * 0.5f;
    m_radius = std::abs((p1 - p0).norm() * 0.5f);
}
Sphere::Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2)
{
    m_creationConstructor = 3;
    Eigen::Vector3f a     = p0 - p2;
    Eigen::Vector3f b     = p1 - p2;

    Eigen::Vector3f aXb = a.cross(b);
    m_radius            = a.norm() * b.norm() * (a - b).norm() / (2.f * aXb.norm());
    m_center            = (a.squaredNorm() * b - b.squaredNorm() * a).cross(aXb) / (2.f * aXb.squaredNorm()) + p2;
}

Sphere::Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& p3)
{
    m_creationConstructor                          = 4;
    Eigen::Matrix<double, 4, 5, Eigen::RowMajor> D = Eigen::Matrix<double, 4, 5, Eigen::RowMajor>::Zero();
    D.col(0)                                       = Eigen::Vector4d(p0.squaredNorm(), p1.squaredNorm(), p2.squaredNorm(), p3.squaredNorm());
    D.block(0, 1, 1, 3)                            = p0.transpose().cast<double>();
    D.block(1, 1, 1, 3)                            = p1.transpose().cast<double>();
    D.block(2, 1, 1, 3)                            = p2.transpose().cast<double>();
    D.block(3, 1, 1, 3)                            = p3.transpose().cast<double>();
    D.col(4)                                       = Eigen::Vector4d::Ones();

    Eigen::Matrix4d                    m = D.block(0, 1, 4, 4);
    Eigen::FullPivLU<Eigen::Matrix4Xd> lu_a(m);
    double                             a = lu_a.determinant();
    m.col(0)                             = D.col(0);
    Eigen::FullPivLU<Eigen::Matrix4Xd> lu_dx(m);
    double                             dx = lu_dx.determinant();
    m.col(1)                              = D.col(1);
    Eigen::FullPivLU<Eigen::Matrix4Xd> lu_dy(m);
    double                             dy = -lu_dy.determinant();
    m.col(2)                              = D.col(2);
    Eigen::FullPivLU<Eigen::Matrix4Xd> lu_dz(m);
    double                             dz = lu_dz.determinant();
    m.col(3)                              = D.col(3);
    Eigen::FullPivLU<Eigen::Matrix4Xd> lu_c(m);
    double                             c = lu_c.determinant();

    double recip = 1.0 / (2.0 * a);
    m_center     = Eigen::Vector3d(dx * recip, dz * recip, dz * recip).cast<float>();
    m_radius     = std::sqrt(dx * dx + dy * dy + dz * dz - 4.0 * a * c) * std::abs(recip);
}

std::pair<bool, Eigen::Vector3f> Sphere::IsHit(const Ray& rRay, Eigen::Vector3f center)
{
    ASSERT(std::fabs(rRay.m_direction.norm() - 1.f) < std::numeric_limits<float>::epsilon());
    Eigen::Vector3f m = rRay.m_origin - center;
    float           b = m.dot(rRay.m_direction);
    float           c = m.dot(m) - m_radius * m_radius;
    if (c > 0.f && b > 0.f)
    {
        return std::pair(false, Eigen::Vector3f());
    }
    float discr = b * b - c;
    if (discr < 0.f)
    {
        return std::pair(false, Eigen::Vector3f());
    }
    float t = -b - std::sqrt(discr);
    if (t < 0.f)
    {
        t = 0.f;
    }
    return std::pair(true, rRay.m_origin + t * rRay.m_direction);
}

std::shared_ptr<Mesh3D> Sphere::CreateMesh()
{
    float x, y, z, xy;                             // vertex position
    float nx, ny, nz, lengthInv = 1.0f / m_radius; // vertex normal

    float sectorStep = 2 * MathHelper::PI / m_sectorCount;
    float stackStep  = MathHelper::PI / m_stackCount;
    float sectorAngle, stackAngle;

    for (uint32_t i = 0; i <= m_stackCount; ++i)
    {
        stackAngle = MathHelper::PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        xy         = m_radius * cosf(stackAngle);        // r * cos(u)
        z          = m_radius * sinf(stackAngle);        // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (uint32_t j = 0; j <= m_sectorCount; ++j)
        {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            m_vertices.push_back(Eigen::Vector3f(x, y, z));

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            m_normals.push_back(Eigen::Vector3f(nx, ny, nz));
        }
    }

    int k1, k2;
    for (uint32_t i = 0; i < m_stackCount; ++i)
    {
        k1 = i * (m_sectorCount + 1); // beginning of current stack
        k2 = k1 + m_sectorCount + 1;  // beginning of next stack

        for (uint32_t j = 0; j < m_sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                m_indices.push_back(k1);
                m_indices.push_back(k2);
                m_indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (m_stackCount - 1))
            {
                m_indices.push_back(k1 + 1);
                m_indices.push_back(k2);
                m_indices.push_back(k2 + 1);
            }
        }
    }

    auto spSphereMesh = std::make_shared<Mesh3D>(m_vertices, m_indices, "Sphere");
    /* auto& vertexNormalAttrib = dynamic_cast<VertexNormalAttribute&>(spSphereMesh->AddVertexAttribute(Mesh3D::EVertexAttribute::Normal)); */
    /* vertexNormalAttrib.SetVertexNormals(m_normals); */
    return spSphereMesh;
}
