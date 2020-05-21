#ifndef AABB_H 
#define AABB_H

#include <Eigen/Dense>
#include <limits>
#include <optional>

class AABB 
{
public:
	struct Intersection
	{
		Intersection(float tmin, const Eigen::Vector3f hitPoint) : m_tmin{tmin}, m_hitPoint{hitPoint}{}
		float m_tmin;
		Eigen::Vector3f m_hitPoint;
	};

public:
	AABB(const Eigen::Vector3f& min, const Eigen::Vector3f& max)
		: m_min{min}, m_max{max} {}

	bool TestABBOverlapping(const AABB& aabb);
	std::optional<Intersection> IntersectRayAABB(Eigen::Vector3f origin, Eigen::Vector3f dir, Eigen::Vector3f translate);
private:
	Eigen::Vector3f m_min;
	Eigen::Vector3f m_max;
};

#endif
