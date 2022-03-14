#include "pch.h"
#include "Skybox.h"
#include "Renderer.h"
#include "Loader.h"

namespace DX = DirectX;

namespace Agata {

	Skybox::Skybox(const SkyboxTexture& one, const SkyboxTexture& two, const SkyboxTexture& three,
		const SkyboxTexture& four, float size, float speed) 
	: m_Texture1(one), m_Texture2(two), m_Texture3(three), m_Texture4(four), m_Speed(speed) {

		m_Mesh = Loader::Get().LoadSkybox(size);
		m_CBO = std::make_shared<ConstantBuffer>(&m_Buffer, sizeof(m_Buffer));

	}

	void Skybox::OnUpdate(float dt) {

		(m_Rotation >= 360.0f) ? m_Rotation = 0.0f : m_Rotation += m_Speed * dt;

	}

	void Skybox::OnRender() {

		m_Buffer.c_Model = DX::XMMatrixRotationY(DX::XMConvertToRadians(m_Rotation));
		m_Buffer.c_View = DX::XMMatrixTranspose(DX::XMMatrixMultiply(m_Buffer.c_Model, Renderer::GetView()));
		m_Buffer.c_Projection = DX::XMMatrixTranspose(Renderer::GetProjection());
		m_Buffer.c_BlendFactor = m_BlendFactor;

		m_CBO->Bind();
		m_CBO->UpdateData(&m_Buffer);

		m_Texture1.Bind(0);
		m_Texture2.Bind(1);
		m_Texture3.Bind(2);
		m_Texture4.Bind(3);

		Renderer::DepthFunc(DepthFuncType::Lequal);
		Renderer::DrawIndexes(m_Mesh.get());
		Renderer::DepthFunc(DepthFuncType::Less);

	}

	float Skybox::GetRotation() const {

		return m_Rotation;

	}

	void Skybox::SetRotation(float rotation) {

		m_Rotation = rotation;

	}

	void Skybox::SetBlendFactor(const DX::XMFLOAT4& blendFactor) {

		m_BlendFactor = blendFactor;

	}


}