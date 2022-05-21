#include "pch.h"
#include "Billboard.h"
#include "Renderer.h"
#include "Loader.h"

namespace Agata {

	Billboard::Billboard(const std::string& path, const std::string& normalPath, DX::XMFLOAT3 position, DX::XMFLOAT3 scale) 
	: Drawable(position, DX::XMFLOAT3(0.0f, 0.0f, 0.0f), scale), m_Texture(path), m_NormalMap(normalPath) {

		m_Mesh = Loader::Get().LoadQuad();
		m_CBO = std::make_shared<ConstantBuffer>(&m_Buffer, sizeof(BillboardBuffer));

	}

	void Billboard::OnUpdate(float dt) {
		
		DX::XMFLOAT3 vecPos = Renderer::GetDirection();
		float angle = std::atan2(vecPos.x, vecPos.z);

		m_Transformation = DX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
			DX::XMMatrixRotationX(0.0f) *
		DX::XMMatrixRotationY(angle) *
		DX::XMMatrixRotationZ(0.0f) * 
		 
		DX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
		
	}

	void Billboard::OnRender() {

		m_Buffer.c_Model = DX::XMMatrixTranspose(m_Transformation);
		m_Buffer.c_View = DX::XMMatrixTranspose(Renderer::GetView());
		m_Buffer.c_Projection = DX::XMMatrixTranspose(Renderer::GetProjection());
		DX::XMFLOAT3 v = Renderer::GetPosition();
		m_Buffer.c_CameraPosition = DX::XMFLOAT4(v.x, v.y, v.z, 1.0f);
		m_Buffer.c_LightPosition = Renderer::GetLight()->GetPosition();
		m_Buffer.c_LightColour = Renderer::GetLight()->GetColour();
		m_CBO->Bind();
		m_CBO->UpdateData(&m_Buffer);

		m_Texture.Bind(0);
		m_NormalMap.Bind(1);

		Renderer::DrawIndexes(m_Mesh.get());

	}


}
