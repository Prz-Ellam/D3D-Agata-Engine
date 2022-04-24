#include "pch.h"
#include "StaticModel.h"
#include "Loader.h"
#include "Renderer.h"

namespace Agata {

	StaticModel::StaticModel(const std::string& modelPath, const DX::XMFLOAT3& position,
		const DX::XMFLOAT3& rotation, const DX::XMFLOAT3& scale, const std::string& diffuseTex,
		const std::string& normalTex, const std::string& specularTex, const DX::XMFLOAT4& ambient,
		const DX::XMFLOAT4& diffuse, const DX::XMFLOAT4& specular, float specularPower,
		bool defaultCollider, const std::vector<BoxCollider>& colliders)
		: Drawable(position, rotation, scale), m_Buffer({}) {

		if (defaultCollider) {
			m_Colliders.clear();
			m_Colliders.push_back({});
			m_Mesh = Loader::Get().LoadStaticModel(modelPath, m_Colliders[0]);
			m_Colliders[0].SetTransformation(m_Transformation);
		}
		else {
			m_Mesh = Loader::Get().LoadStaticModel(modelPath);
			m_Colliders = colliders;
			for (auto& collider : m_Colliders) {
				collider.SetTransformation(m_Transformation);
			}
		}
		
		m_CBO = std::make_shared<ConstantBuffer>(&m_Buffer, sizeof(m_Buffer));
		m_Material = std::make_shared<Material>(diffuseTex, normalTex, specularTex, ambient, 
			diffuse, specular, specularPower);

	}

	void StaticModel::CheckCollision(std::unique_ptr<Camera>& camera) {

		for (auto& collider : m_Colliders) {
			if (collider.IsColliding(camera->GetPosition())) {
				camera->Back();
			}
		}

	}

	void StaticModel::OnRender() {

		m_Buffer.c_Model = DX::XMMatrixTranspose(m_Transformation);
		m_Buffer.c_View = DX::XMMatrixTranspose(Renderer::GetView());
		m_Buffer.c_Projection = DX::XMMatrixTranspose(Renderer::GetProjection());
		DX::XMFLOAT3 v = Renderer::GetPosition();
		m_Buffer.c_CameraPosition = DX::XMFLOAT4(v.x, v.y, v.z, 1.0f);
		m_Buffer.c_LightPosition = Renderer::GetLight()->GetPosition();
		m_Buffer.c_LightColour = Renderer::GetLight()->GetColour();
		m_CBO->Bind();
		m_CBO->UpdateData(&m_Buffer);

		m_Material->BindDiffuseTexture(0);
		m_Material->BindNormalTexture(1);

		Renderer::DrawIndexes(m_Mesh.get());

	}

	StaticModelBuilder::StaticModelBuilder() : 
		m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 0.0f, 0.0f), m_Scale(0.0f, 0.0f, 0.0f) {
	}

	StaticModelBuilder StaticModelBuilder::GenerateParams() {

		return StaticModelBuilder();

	}

	StaticModelBuilder& StaticModelBuilder::ModelPath(const std::string& modelPath) {

		m_ModelPath = modelPath;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::DiffuseTexture(const std::string& path) {

		m_DiffuseTexture = path;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::SpecularTexture(const std::string& path) {

		m_SpecularTexture = path;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::NormalTexture(const std::string& path) {

		m_NormalTexture = path;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::Position(const DX::XMFLOAT3& position) {

		m_Position = position;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::Rotation(const DX::XMFLOAT3& rotation) {

		m_Rotation = rotation;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::Scale(const DX::XMFLOAT3& scale) {

		m_Scale = scale;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::AmbientMaterial(const DX::XMFLOAT4& ambient) {

		m_AmbientMaterial = ambient;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::DiffuseMaterial(const DX::XMFLOAT4& diffuse) {

		m_DiffuseMaterial = diffuse;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::SpecularMaterial(const DX::XMFLOAT4& specular) {

		m_SpecularMaterial = specular;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::SpecularPowerMaterial(float specularPower) {

		m_SpecularPowerMaterial = specularPower;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::DefaultCollider(bool defaultCollider) {

		m_DefaultCollider = defaultCollider;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::AddCollider(const BoxCollider& collider) {

		m_BoxCollider.push_back(collider);
		collidersCount++;
		return *this;

	}

	StaticModelBuilder& StaticModelBuilder::AddCollider(const std::string& filePath) {

		m_BoxCollider.push_back({});
		m_BoxCollider[collidersCount].SetAttribs(filePath);
		collidersCount++;
		return *this;

	}

	std::shared_ptr<StaticModel> StaticModelBuilder::Build() {

		return std::make_shared<StaticModel>(m_ModelPath, m_Position, m_Rotation, m_Scale, 
			m_DiffuseTexture, m_NormalTexture, m_SpecularTexture, m_AmbientMaterial, m_DiffuseMaterial,
			m_SpecularMaterial, m_SpecularPowerMaterial, m_DefaultCollider, m_BoxCollider);

	}

}