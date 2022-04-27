#ifndef AGATA_D11_STATIC_MODEL_H
#define AGATA_D11_STATIC_MODEL_H
#pragma once

#include "ConstantBuffer.h"
#include "Mesh.h"
#include "BoxCollider.h"
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

	HLSL struct StaticModelBuffer2 {
		DX::XMFLOAT4 c_AmbientMaterial;
		DX::XMFLOAT4 c_DiffuseMaterial;
		DX::XMFLOAT4 c_SpecularMaterial;
		FLOAT c_Shininess;
	};

	class StaticModel : public Drawable {
	public:
		StaticModel() = default;
		StaticModel(const std::string& modelPath, const DX::XMFLOAT3& position, 
			const DX::XMFLOAT3& rotation, const DX::XMFLOAT3& scale, const std::string& diffuseTex,
			const std::string& normalTex, const std::string& specularTex, const DX::XMFLOAT4& ambient,
			const DX::XMFLOAT4& diffuse, const DX::XMFLOAT4& specular, float specularPower,
			bool defaultCollider, const std::vector<BoxCollider>&  collider);
		StaticModel(const StaticModel& other) = default;
		StaticModel(StaticModel&& other) noexcept = default;
		virtual ~StaticModel() = default;

		void CheckCollision(std::unique_ptr<Camera>& camera);

		StaticModel& operator=(const StaticModel& other) = default;
		StaticModel& operator=(StaticModel&& other) noexcept = default;

		std::shared_ptr<Material>& GetMaterial();

		void OnRender() override;

	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Material> m_Material;
		std::shared_ptr<ConstantBuffer> m_CBO;
		std::shared_ptr<ConstantBuffer> m_CBO2;
		StaticModelBuffer m_Buffer;
		StaticModelBuffer2 m_Buffer2;

		std::vector<BoxCollider> m_Colliders;
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
		StaticModelBuilder& DefaultCollider(bool);
		StaticModelBuilder& AddCollider(const BoxCollider&);
		StaticModelBuilder& AddCollider(const std::string&);
		std::shared_ptr<StaticModel> Build();
	private:
		std::string m_ModelPath;
		std::string m_DiffuseTexture;
		std::string m_SpecularTexture;
		std::string m_NormalTexture;
		DX::XMFLOAT3 m_Position = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DX::XMFLOAT3 m_Rotation = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DX::XMFLOAT3 m_Scale = DX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		DX::XMFLOAT4 m_AmbientMaterial = DX::XMFLOAT4(0.15f, 0.15f, 0.15f, 0.15f);
		DX::XMFLOAT4 m_DiffuseMaterial = DX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		DX::XMFLOAT4 m_SpecularMaterial = DX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		float m_SpecularPowerMaterial = 32;
		bool m_DefaultCollider = false;
		std::vector<BoxCollider> m_BoxCollider;
		int collidersCount = 0;
	};

}

#endif
