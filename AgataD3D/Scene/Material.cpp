#include "pch.h"
#include "Material.h"

namespace Agata {

	Material::Material(const std::string& diffuseTex, const std::string& normalTex,
		const std::string& specularTex, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse,
		DirectX::XMFLOAT4 specular, float specularPower) 
	: m_DiffuseTexture(diffuseTex, DefaultTexture::DiffuseMap), 
		m_NormalTexture(normalTex, DefaultTexture::NormalMap), 
		m_SpecularTexture(specularTex, DefaultTexture::SpecularMap),
	m_AmbientMaterial(ambient), m_DiffuseMaterial(diffuse), m_SpecularMaterial(specular), 
	m_SpecularPowerMaterial(specularPower) {}

	void Material::BindDiffuseTexture(uint32_t slot) {

		m_DiffuseTexture.Bind(slot);

	}

	void Material::BindNormalTexture(uint32_t slot) {

		m_NormalTexture.Bind(slot);

	}

	void Material::BindSpecularTexture(uint32_t slot) {

		m_SpecularTexture.Bind(slot);

	}

	DirectX::XMFLOAT4 Material::GetAmbient() const {

		return m_AmbientMaterial;

	}

	DirectX::XMFLOAT4 Material::GetDiffuse() const {

		return m_DiffuseMaterial;

	}

	DirectX::XMFLOAT4 Material::GetSpecular() const {

		return m_SpecularMaterial;

	}

	float Material::GetShininess() const {

		return m_SpecularPowerMaterial;

	}

	void Material::SetAmbient(DirectX::XMFLOAT4 ambient) {

		m_AmbientMaterial = ambient;

	}

	void Material::SetDiffuse(DirectX::XMFLOAT4 diffuse) {

		m_DiffuseMaterial = diffuse;

	}

	void Material::SetSpecular(DirectX::XMFLOAT4 specular) {

		m_SpecularMaterial = specular;

	}

	void Material::SetShininess(FLOAT shininess) {

		m_SpecularPowerMaterial = shininess;

	}

}