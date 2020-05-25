#ifndef QUADER_H
#define QUADER_H

#include "Primitives3D.h"
#include <Eigen/Dense>
#include <memory>
#include <vector>
#include <AABB.h>
#include <utility>
#include "Ray.h"

class Quader : public Primitives3D
{
public:
    explicit Quader(float width, float height, float depth):
	  m_width{width}
    , m_height{height}
    , m_depth{depth}
    , m_aabb{Eigen::Vector3f(-m_width * 0.5f, -m_height * 0.5f, -m_depth *0.5f),
		Eigen::Vector3f(m_width * 0.5f, m_height * 0.5f, m_depth *0.5f)}

	{}
	explicit Quader(Eigen::Vector3f min, Eigen::Vector3f max) :
		m_width{max[0] - min[0]},
		m_height{max[1] - min[1]},
		m_depth{max[2] - min[2]},
		m_aabb{min, max}
		{}

    std::shared_ptr<Mesh3D> CreateMesh() override;
	std::pair<bool, Eigen::Vector3f> IsHit(const Ray& rRay, Eigen::Vector3f position);
	
private:
    float                        m_width  = 1.f;
    float                        m_height = 1.f;
    float                        m_depth  = 1.f;
	AABB m_aabb;
    std::vector<Eigen::Vector3f> m_vertices;
    std::vector<uint32_t>        m_indices;
};

#endif
