#ifndef AGATA_D11_SKYBOX_H
#define AGATA_D11_SKYBOX_H
#pragma once

#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Texture3D.h"
#include <memory>
#include <DirectXMath.h>

namespace DX = DirectX;

namespace Agata {

	HLSL struct SkyboxBuffer {
		DX::XMMATRIX c_Model;
		DX::XMMATRIX c_View;
		DX::XMMATRIX c_Projection;
		DX::XMFLOAT4 c_BlendFactor;
	};

	class Skybox {
	public:
		Skybox(const SkyboxTexture& one, const SkyboxTexture& two, const SkyboxTexture& three,
			const SkyboxTexture& four, float size, float speed);
		Skybox(const Skybox& rhs) = default;
		Skybox(Skybox&& rhs) noexcept = default;
		virtual ~Skybox() = default;

		Skybox& operator=(const Skybox& rhs) = default;
		Skybox& operator=(Skybox&& rhs) = default;

		void OnUpdate(float dt);
		void OnRender();

		float GetRotation() const;
		void SetRotation(float rotation);
		void SetBlendFactor(const DX::XMFLOAT4& blendFactor);
	private:
		Texture3D m_Texture1;
		Texture3D m_Texture2;
		Texture3D m_Texture3;
		Texture3D m_Texture4;
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<ConstantBuffer> m_CBO;
		SkyboxBuffer m_Buffer;
		float m_Rotation = 0.0f, m_Speed;
		DX::XMFLOAT4 m_BlendFactor = DX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	};

}

#endif
