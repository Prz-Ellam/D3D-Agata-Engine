#include "pch.h"
#include "Billboard.h"
#include "Renderer.h"

namespace Agata {

	Billboard::Billboard(const std::string path, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale) 
	: Drawable(position, position, scale) {



	}

	void Billboard::OnRender() {

		m_CBO->Bind();

		Renderer::DrawIndexes(m_Mesh.get());

	}


}
