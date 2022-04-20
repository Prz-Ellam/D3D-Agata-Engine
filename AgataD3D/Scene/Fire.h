#ifndef AGATA_D11_FIRE_H
#define AGATA_D11_FIRE_H
#pragma once

#include "Drawable.h"
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Texture2D.h"

namespace Agata {

	HLSL struct FireBuffer {
		DX::XMMATRIX c_Model;
		DX::XMMATRIX c_View;
		DX::XMMATRIX c_Projection;
		float c_Displacement;
	};

	class Fire : public Drawable {
	public:
		Fire(const std::string& rampPath, const std::string& alphaPath, const std::string& dudvPath, 
			const std::string& noisePath, const DX::XMFLOAT3& position, const DX::XMFLOAT3& scale);
		Fire(const Fire& other) = delete;
		Fire(Fire&& other) noexcept = delete;
		virtual ~Fire() = default;

		Fire& operator=(const Fire& other) = default;
		Fire& operator=(Fire&& other) = default;

		void OnUpdate(float dt);
		void OnRender() override;
	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<ConstantBuffer> m_CBO;
		Texture2D m_Ramp;
		Texture2D m_Alpha;
		Texture2D m_Dudv;
		Texture2D m_Noise;
		FireBuffer m_Buffer;
		float m_Displacement;
	};

}

#endif