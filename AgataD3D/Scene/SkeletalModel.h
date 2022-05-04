#ifndef AGATA_D11_SKELETAL_MODEL_H
#define AGATA_D11_SKELETAL_MODEL_H
#pragma once

#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Material.h"
#include "Drawable.h"
#include "BoxCollider.h"
#include "Loader.h"
#include <memory>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Agata {

	HLSL struct SkeletalModelBuffer {
		DX::XMMATRIX c_Model;
		DX::XMMATRIX c_View;
		DX::XMMATRIX c_Projection;
		DX::XMFLOAT4 c_CameraPosition;
		DX::XMFLOAT4 c_LightPosition;
		DX::XMFLOAT4 c_LightColour;
		DX::XMMATRIX c_Joints[128];
	};

	HLSL struct SkeletalModelBuffer2 {
		DX::XMFLOAT4 c_AmbientMaterial;
		DX::XMFLOAT4 c_DiffuseMaterial;
		DX::XMFLOAT4 c_SpecularMaterial;
		FLOAT c_Shininess;
	};

	class SkeletalModel : public Drawable {
	public:
		SkeletalModel() = default;
		SkeletalModel(const std::string& filePath, const std::string& diffuseTex, 
			const std::string& specularTex, const std::string& normalTex,
			const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation, 
			const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT4& ambientMaterial, 
			const DirectX::XMFLOAT4& diffuseMaterial, const DirectX::XMFLOAT4& specularMaterial,
			float shininessMaterial);
		SkeletalModel(const SkeletalModel& other) = delete;
		SkeletalModel(SkeletalModel&& other) = delete;
		virtual ~SkeletalModel() = default;

		SkeletalModel& operator=(const SkeletalModel& other) = default;
		SkeletalModel& operator=(SkeletalModel&& other) noexcept = default;

		void OnUpdate(float dt);
		void OnRender() override;

		std::shared_ptr<Material>& GetMaterial();
	private:
		//SkeletalModelProps m_Properties;
		std::shared_ptr<Material> m_Material;
		std::shared_ptr<ConstantBuffer> m_CBO;
		std::shared_ptr<ConstantBuffer> m_CBO2;
		SkeletalModelBuffer m_Buffer;
		SkeletalModelBuffer2 m_Buffer2;
		std::vector<DirectX::XMMATRIX> m_Transformations;

		Assimp::Importer m_Importer;
		const aiScene* m_Scene;
		std::vector<Mesh> m_Meshes;
		std::map<std::string, uint32_t> m_JointInfo;
		std::vector< std::pair<DirectX::XMMATRIX, DirectX::XMMATRIX>> m_JointTransformations;
		std::map<std::string, const aiNodeAnim*> m_AnimationInfo;
		float m_TicksPerSec;
		DirectX::XMMATRIX m_GlobalInverseTransformation;
		uint32_t m_JointCount = 0;

		void LoadSkeletalModel(const std::string& path);
		void ProcessNode(const aiScene* const scene, aiNode* node);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<AnimVertex>& vertices, std::vector<uint32_t>& indices);

		void LoadJointsTransforms(float timeStamp, std::vector<DirectX::XMMATRIX>& transformations);
		void FindTransformations(float timeStamp, const aiNode* node, 
			const DirectX::XMMATRIX parentTransformation = DirectX::XMMatrixIdentity());

		DirectX::XMVECTOR LerpPosition(float animationTime, const aiNodeAnim* node);
		DirectX::XMVECTOR LerpRotation(float animationTime, const aiNodeAnim* node);
		DirectX::XMVECTOR LerpScale(float animationTime, const aiNodeAnim* node);
	};

	class SkeletalModelBuilder {
	public:
		SkeletalModelBuilder();

		static SkeletalModelBuilder GenerateParams();

		SkeletalModelBuilder& ModelPath(const std::string& path);
		SkeletalModelBuilder& DiffuseTexture(const std::string& path);
		SkeletalModelBuilder& SpecularTexture(const std::string& path);
		SkeletalModelBuilder& NormalTexture(const std::string& path);
		SkeletalModelBuilder& Position(const DX::XMFLOAT3& position);
		SkeletalModelBuilder& Rotation(const DX::XMFLOAT3& rotation);
		SkeletalModelBuilder& Scale(const DX::XMFLOAT3& scale);
		SkeletalModelBuilder& AmbientMaterial(const DX::XMFLOAT4& ambient);
		SkeletalModelBuilder& DiffuseMaterial(const DX::XMFLOAT4& diffuse);
		SkeletalModelBuilder& SpecularMaterial(const DX::XMFLOAT4& specular);
		SkeletalModelBuilder& SpecularPowerMaterial(float specularPower);
		SkeletalModelBuilder& DefaultCollider(bool);
		SkeletalModelBuilder& AddCollider(const BoxCollider&);
		SkeletalModelBuilder& AddCollider(const std::string&);
		std::shared_ptr<SkeletalModel> Build();
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
