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
		FLOAT c_Alpha;
	};

	class GUI : public Drawable {
	public:
		GUI() = default;
		GUI(const std::string& path, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale);
		GUI(const GUI& other) = delete;
		GUI(GUI&& other) noexcept = delete;

		GUI& operator=(const GUI& other) = delete;
		GUI& operator=(GUI&& other) noexcept = delete;

		void OnRender() override;
		void OnRender(bool range, float dt);
	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<ConstantBuffer> m_CBO;
		Texture2D m_Texture;
		GUIBuffer m_Buffer;
		float m_Time = 0.0f;
		float m_Alpha = 0.0f;
	};

}

#endif
