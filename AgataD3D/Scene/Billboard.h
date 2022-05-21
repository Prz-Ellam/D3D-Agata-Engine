#ifndef AGATA_D3D_BILLBOARD_H
#define AGATA_D3D_BILLBOARD_H
#pragma once

#include "Drawable.h"
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Texture2D.h"

namespace Agata {

	HLSL struct BillboardBuffer {
		DX::XMMATRIX c_Model;
		DX::XMMATRIX c_View;
		DX::XMMATRIX c_Projection;
		DX::XMFLOAT4 c_CameraPosition;
		DX::XMFLOAT4 c_LightPosition;
		DX::XMFLOAT4 c_LightColour;
	};

	class Billboard : public Drawable {
	public:
		Billboard(const std::string& path, const std::string& normalPath, DX::XMFLOAT3 position, DX::XMFLOAT3 scale);
		Billboard(const Billboard& other) = default;
		Billboard(Billboard&& other) noexcept = default;
		virtual ~Billboard() = default;

		Billboard& operator=(const Billboard& other) = default;
		Billboard& operator=(Billboard&& other) noexcept = default;

		void OnUpdate(float dt);
		void OnRender() override;
	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<ConstantBuffer> m_CBO;
		Texture2D m_Texture;
		Texture2D m_NormalMap;
		BillboardBuffer m_Buffer;
	};

}

#endif
