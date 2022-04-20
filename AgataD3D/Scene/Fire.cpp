#include "pch.h"
#include "Fire.h"
#include "Loader.h"

namespace Agata {

	Fire::Fire(const std::string& rampPath, const std::string& alphaPath, const std::string& dudvPath,
		const std::string& noisePath, const DX::XMFLOAT3& position, const DX::XMFLOAT3& scale) :
		Drawable(position, DX::XMFLOAT3(0.0f, 0.0f, 0.0f), scale), m_Ramp(rampPath), m_Alpha(alphaPath), 
		m_Dudv(dudvPath), m_Noise(noisePath) {

		m_Mesh = Loader::Get().LoadQuad();
		m_CBO = std::make_shared<ConstantBuffer>(&m_Buffer, sizeof(FireBuffer));
		m_Displacement = 0.0f;

	}

	void Fire::OnUpdate(float dt) {

		DX::XMFLOAT3 vecPos = Renderer::GetDirection();
		float angle = std::atan2(vecPos.x, vecPos.z);

		m_Transformation = DX::XMMatrixRotationX(0.0f) *
			DX::XMMatrixRotationY(angle) *
			DX::XMMatrixRotationZ(0.0f) * 
			DX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
			DX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

		(m_Displacement < 1.0f) ? m_Displacement += 0.05f * dt : m_Displacement = 0.0f;

	}

	void Fire::OnRender() {

		m_Buffer.c_Model = DX::XMMatrixTranspose(m_Transformation);
		m_Buffer.c_View = DX::XMMatrixTranspose(Renderer::GetView());
		m_Buffer.c_Projection = DX::XMMatrixTranspose(Renderer::GetProjection());
		m_Buffer.c_Displacement = m_Displacement;
		m_CBO->Bind();
		m_CBO->UpdateData(&m_Buffer);

		m_Noise.Bind(0);
		m_Alpha.Bind(1);
		m_Ramp.Bind(2);
		m_Dudv.Bind(3);

		Renderer::DrawIndexes(m_Mesh.get());

	}

}