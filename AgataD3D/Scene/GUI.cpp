#include "pch.h"
#include "GUI.h"
#include "Renderer.h"
#include "Loader.h"
#include <d3d11.h>

namespace Agata {

	GUI::GUI(const std::string& path, float width, float height) : 
		Drawable(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(.5f, .5f, .5f)), 
	    m_Texture(path) {

		m_Mesh = Loader::Get().LoadQuad();
		m_CBO = std::make_shared<ConstantBuffer>(&m_Buffer, sizeof(GUIBuffer));

	}

	void GUI::OnRender() {

		m_Buffer.c_Model = DX::XMMatrixTranspose(m_Transformation);
		m_CBO->Bind();
		m_CBO->UpdateData(&m_Buffer);

		m_Texture.Bind(0);

		Renderer::DrawIndexes(m_Mesh.get());

	}

}