#ifndef MATERIAL_H
#define MATERIAL_H

#include <Eigen/Dense>
#include "Shader.h"
#include "Macros.h"
#include <memory>

enum class MaterialType
{
	BRONZE,
	GOLD,
	GREEN_RUBBER,
	RED_PLASTIC
};

struct Material
{
	Material(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, float shininess) :
		m_ambient{ambient}, m_diffuse{diffuse}, m_specular{specular}, m_shininess{shininess} {}

	void Activate(Shader* pShader) const
	{
        pShader->SetVector("material.ambient", m_ambient);
        pShader->SetVector("material.diffuse", m_diffuse);
        pShader->SetVector("material.specular", m_specular);
        pShader->SetValue("material.shininess", m_shininess);
	}

        static std::shared_ptr<Material> GetMaterial(const MaterialType& materialType)
        {
		switch(materialType)
		{
			case MaterialType::BRONZE: 
				{
					return std::make_shared<Material>(
							Eigen::Vector3f{.2125f, .1275f, .054f}, 
							Eigen::Vector3f{.714f, .4284f, .18144f}, 
							Eigen::Vector3f{.393548f, .271906f, .166721f}, 
							.2f); 
				}
				break;
			case MaterialType::GOLD: 
				{
					return std::make_shared<Material>(
							Eigen::Vector3f{.24725f, .1995, .0745f}, 
							Eigen::Vector3f{.75164f, .60648f, .22648f}, 
							Eigen::Vector3f{.628281f, .555802f, .366065f}, 
							.4f); 
				}
				break;
			case MaterialType::GREEN_RUBBER: 
				{
					return std::make_shared<Material>(
							Eigen::Vector3f{.0f, .05f, .05f}, 
							Eigen::Vector3f{.4f, .5f, .5f}, 
							Eigen::Vector3f{.04f, .7f, .7f}, 
							.078125f); 
				}
				break;
			case MaterialType::RED_PLASTIC: 
				{
					return std::make_shared<Material>(
							Eigen::Vector3f{.0f, 0.f, 0.f}, 
							Eigen::Vector3f{.5f, 0.f, 0.f}, 
							Eigen::Vector3f{.7f, .6f, .6f}, 
							.25f); 
				}
				break;
			default:
				ASSERT(0);
		}

		return nullptr;
	}

    private:
        const Eigen::Vector3f m_ambient;
        const Eigen::Vector3f m_diffuse;
        const Eigen::Vector3f m_specular;
        const float           m_shininess;
};

#endif 
