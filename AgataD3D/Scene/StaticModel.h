#ifndef AGATA_D11_STATIC_MODEL_H
#define AGATA_D11_STATIC_MODEL_H
#pragma once

#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Drawable.h"
#include "Material.h"

namespace DX = DirectX;

namespace Agata {

	HLSL struct StaticModelBuffer {
		DX::XMMATRIX c_Model;
		DX::XMMATRIX c_View;
		DX::XMMATRIX c_Projection;
		DX::XMFLOAT4 c_CameraPosition;
		DX::XMFLOAT4 c_LightPosition;
		DX::XMFLOAT4 c_LightColour;
	};

	class StaticModel : public Drawable {
	public:
		StaticModel() = default;
		StaticModel(const std::string& modelPath, const DX::XMFLOAT3& position, 
			const DX::XMFLOAT3& rotation, const DX::XMFLOAT3& scale, const std::string& diffuseTex,
			const std::string& normalTex, const std::string& specularTex, const DX::XMFLOAT4& ambient,
			const DX::XMFLOAT4& diffuse, const DX::XMFLOAT4& specular, float specularPower);
		StaticModel(const StaticModel& other) = default;
		StaticModel(StaticModel&& other) noexcept = default;
		virtual ~StaticModel() = default;

		StaticModel& operator=(const StaticModel& other) = default;
		StaticModel& operator=(StaticModel&& other) noexcept = default;

		void OnRender() override;

	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Material> m_Material;
		std::shared_ptr<ConstantBuffer> m_CBO;
		StaticModelBuffer m_Buffer;
	};

	class StaticModelBuilder {
	public:
		StaticModelBuilder();

		static StaticModelBuilder GenerateParams();

		StaticModelBuilder& ModelPath(const std::string& path);
		StaticModelBuilder& DiffuseTexture(const std::string& path);
		StaticModelBuilder& SpecularTexture(const std::string& path);
		StaticModelBuilder& NormalTexture(const std::string& path);
		StaticModelBuilder& Position(const DX::XMFLOAT3& position);
		StaticModelBuilder& Rotation(const DX::XMFLOAT3& rotation);
		StaticModelBuilder& Scale(const DX::XMFLOAT3& scale);
		StaticModelBuilder& AmbientMaterial(const DX::XMFLOAT4& ambient);
		StaticModelBuilder& DiffuseMaterial(const DX::XMFLOAT4& diffuse);
		StaticModelBuilder& SpecularMaterial(const DX::XMFLOAT4& specular);
		StaticModelBuilder& SpecularPowerMaterial(float specularPower);
		/*ModelBuilder& DefaultCollider(bool);
		ModelBuilder& Collider(const BoxCollider&);
		ModelBuilder& Collider(const std::string&);
		StaticModelBuilder Build();
		Model* BuildNew();
		*/
		std::shared_ptr<StaticModel> Build();
	private:
		std::string m_ModelPath;
		std::string m_DiffuseTexture;
		std::string m_SpecularTexture;
		std::string m_NormalTexture;
		DX::XMFLOAT3 m_Position;
		DX::XMFLOAT3 m_Rotation;
		DX::XMFLOAT3 m_Scale;
		DX::XMFLOAT4 m_AmbientMaterial = DX::XMFLOAT4(0.15f, 0.15f, 0.15f, 0.15f);
		DX::XMFLOAT4 m_DiffuseMaterial = DX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		DX::XMFLOAT4 m_SpecularMaterial = DX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		float m_SpecularPowerMaterial = 32;

		//bool defaultCollider = false;
		//BoxCollider boxCollider;
	};

}

#endif
