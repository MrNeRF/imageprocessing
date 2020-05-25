#include "AABB.h"
#include <iostream>

bool AABB::TestABBOverlapping(const AABB& aabb)
{
	if(m_max[0] < aabb.m_min[0] || m_min[0] > aabb.m_max[0])	
		return false;
	if(m_max[1] < aabb.m_min[1] || m_min[1] > aabb.m_max[1])	
		return false;
	if(m_max[2] < aabb.m_min[2] || m_min[2] > aabb.m_max[2])	
		return false;
	return true;
}

std::optional<AABB::Intersection> AABB::IntersectRayAABB(Eigen::Vector3f origin, Eigen::Vector3f dir, Eigen::Vector3f translate)
{
	float tmin = 0.f; 
	float tmax = std::numeric_limits<float>::max();

	Eigen::Vector3f min = m_min + translate;
	Eigen::Vector3f max = m_max + translate;

	for(uint32_t i = 0; i  < 3; ++i)
	{
		if(std::abs(dir[i]) < std::numeric_limits<float>::epsilon())
		{
			if(origin[i] < min[i] || origin[i] > max[i])
				return std::nullopt;
		}
		else
		{
			float recip = 1.f / dir[i];
			float t1 = (min[i] - origin[i]) * recip;
			float t2 = (max[i] - origin[i]) * recip;
			if (t1 > t2)
			{
				std::swap(t1, t2);
			}

			tmin = std::max(tmin, t1);
			tmax = std::min(tmax, t2);
			if (tmin > tmax)
			{
				return std::nullopt;
			}
		}
	}
	std::cout << "Hit!!!! "  << tmin << "at: " << origin + tmin * dir << std::endl;
	return Intersection(tmin,  origin + tmin * dir);
}
