#ifndef AGATA_D3D_GUI_H
#define AGATA_D3D_GUI_H
#pragma once

#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Drawable.h"
#include "Texture2D.h"
#include <memory>

namespace Agata {

	HLSL struct GUIBuffer {
		DX::XMMATRIX c_Model;
	};

	class GUI : public Drawable {
	public:
		GUI() = default;
		GUI(const std::string& path, float width, float height);
		GUI(const GUI& other) = delete;
		GUI(GUI&& other) noexcept = delete;

		GUI& operator=(const GUI& other) = delete;
		GUI& operator=(GUI&& other) noexcept = delete;

		void OnRender() override;
	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<ConstantBuffer> m_CBO;
		Texture2D m_Texture;
		GUIBuffer m_Buffer;

	};

}

#endif
