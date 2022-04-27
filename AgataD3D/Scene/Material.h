#ifndef AGATA_D11_MATERIAL_H
#define AGATA_D11_MATERIAL_H
#pragma once

#include "Texture2D.h"

namespace Agata {

	class Material {
	public:
		Material(const std::string& diffuseTex, const std::string& normalTex, const std::string& specularTex,
			DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, float
		specularPower);

		void BindDiffuseTexture(uint32_t slot);
		void BindNormalTexture(uint32_t slot);
		void BindSpecularTexture(uint32_t slot);

		DirectX::XMFLOAT4 GetAmbient() const;
		DirectX::XMFLOAT4 GetDiffuse() const;
		DirectX::XMFLOAT4 GetSpecular() const;
		float GetShininess() const;

		void SetAmbient(DirectX::XMFLOAT4 ambient);
		void SetDiffuse(DirectX::XMFLOAT4 diffuse);
		void SetSpecular(DirectX::XMFLOAT4 specular);
		void SetShininess(FLOAT shininess);

	private:
		Texture2D m_DiffuseTexture;
		Texture2D m_NormalTexture;
		Texture2D m_SpecularTexture;
		DirectX::XMFLOAT4 m_AmbientMaterial;
		DirectX::XMFLOAT4 m_DiffuseMaterial;
		DirectX::XMFLOAT4 m_SpecularMaterial;
		float m_SpecularPowerMaterial;
	};

}

#endif
