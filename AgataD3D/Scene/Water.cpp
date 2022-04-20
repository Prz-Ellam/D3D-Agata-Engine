#include "pch.h"
#include "Water.h"
#include "Loader.h"

namespace Agata {

	Water::Water(const DX::XMFLOAT3& position, const DX::XMFLOAT3& rotation, 
		const DX::XMFLOAT3& scale, const std::string& dudvMap,
		const std::string& normalMap, uint32_t width, uint32_t height) 
		: Drawable(position, position, scale), m_MoveFactor(0.0f), m_Refraction(width, height), 
		m_Reflection(width, height), m_DudvMap(dudvMap), m_NormalMap(normalMap) {

		m_Mesh = Loader::Get().LoadHorizontalQuad();
		m_CBO = std::make_shared<ConstantBuffer>(&m_Buffer, sizeof(WaterBuffer));

	}

	void Water::OnUpdate(double dt) {

		m_Transformation = DX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z)
			* DX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

		m_MoveFactor = std::fmod(m_MoveFactor + dt * 0.01f, 1.0f);

	}

	void Water::BeginRefraction() {

		m_Refraction.BindFrameBuffer();

	}

	void Water::BeginReflection() {

		m_Reflection.BindFrameBuffer();

	}

	void Water::EndRefraction() {

		m_Refraction.UnbindFramebuffer();

	}

	void Water::EndReflection() {

		m_Reflection.UnbindFramebuffer();

	}

	void Water::OnRender() {

		m_Buffer.c_Model = DX::XMMatrixTranspose(m_Transformation);
		m_Buffer.c_View = DX::XMMatrixTranspose(Renderer::GetView());
		m_Buffer.c_Projection = DX::XMMatrixTranspose(Renderer::GetProjection());
		m_Buffer.c_Colour = m_Colour;
		DX::XMFLOAT3 v = Renderer::GetPosition();
		m_Buffer.c_CameraPos = DX::XMFLOAT4(v.x, v.y, v.z, 1.0f);
		m_Buffer.c_LightPosition = Renderer::GetLight()->GetPosition();
		m_Buffer.c_LightColour = Renderer::GetLight()->GetColour();
		m_Buffer.c_Displacement = m_MoveFactor;
		m_CBO->Bind();
		m_CBO->UpdateData(&m_Buffer);

		m_Reflection.BindTexture(0u);
		m_Refraction.BindTexture(1u);
		m_DudvMap.Bind(2u);
		m_NormalMap.Bind(3u);

		Renderer::DrawIndexes(m_Mesh.get());

	}


	WaterBuilder WaterBuilder::GenerateParams() {

		return WaterBuilder();

	}

	WaterBuilder& WaterBuilder::Position(const DX::XMFLOAT3& position) {

		m_Position = position;
		return *this;

	}

	WaterBuilder& WaterBuilder::Scale(const DX::XMFLOAT3& scale) {

		m_Scale = scale;
		return *this;

	}

	WaterBuilder& WaterBuilder::DuDv(const std::string& dudvMap) {

		m_DudvMap = dudvMap;
		return *this;

	}

	WaterBuilder& WaterBuilder::NormalMap(const std::string& normalMap) {

		m_NormalMap = normalMap;
		return *this;

	}

	WaterBuilder& WaterBuilder::Width(uint32_t width) {

		m_Width = width;
		return *this;

	}

	WaterBuilder& WaterBuilder::Height(uint32_t height) {

		m_Height = height;
		return *this;

	}

	std::shared_ptr<Water>  WaterBuilder::Build() {

		return nullptr;
		//return std::make_shared<Water>(m_Position, m_Scale, m_DudvMap, m_NormalMap, m_Width, m_Height);

	}

}