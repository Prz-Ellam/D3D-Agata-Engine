#ifndef AGATA_D11_WATER_H
#define AGATA_D11_WATER_H
#pragma once

#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Framebuffer.h"
#include "Drawable.h"

namespace DX = DirectX;

namespace Agata {
	
	HLSL struct WaterBuffer {
		DX::XMMATRIX c_Model;
		DX::XMMATRIX c_View;
		DX::XMMATRIX c_Projection;
		DX::XMFLOAT4 c_Colour;
		DX::XMFLOAT3 c_CameraPos;
	};

	class Water : public Drawable {
	public:
		friend class WaterBuilder;
		Water(const DX::XMFLOAT3& position, const DX::XMFLOAT3& rotation, 
			const DX::XMFLOAT3& scale, const std::string& dudvMap, 
			const std::string& normalMap,uint32_t width, uint32_t height);
		Water(const Water& rhs) = delete;
		Water(Water&& rhs) noexcept = delete;
		virtual ~Water() = default;

		Water& operator=(const Water& rhs) = default;
		Water& operator=(Water&& rhs) noexcept = default;

		void BeginRefraction();
		void BeginReflection();

		void EndRefraction();
		void EndReflection();

		void OnUpdate(double dt);
		void OnRender();
	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<ConstantBuffer> m_CBO;
		WaterBuffer m_Buffer;

		uint32_t m_Width, m_Height;

		Framebuffer m_Refraction, m_Reflection;

		float m_MoveFactor;
		DX::XMFLOAT4 m_Colour = DX::XMFLOAT4(0.0f, 0.3f, 0.8f, 1.0f);
	};

	class WaterBuilder {
	public:
		WaterBuilder() = default;

		static WaterBuilder GenerateParams();

		WaterBuilder& Position(const DX::XMFLOAT3& position);
		WaterBuilder& Scale(const DX::XMFLOAT3& scale);
		WaterBuilder& DuDv(const std::string& dudvMap);
		WaterBuilder& NormalMap(const std::string& normalMap);
		WaterBuilder& Width(uint32_t width);
		WaterBuilder& Height(uint32_t height);

		std::shared_ptr<Water> Build();
	private:
		DX::XMFLOAT3 m_Position;
		DX::XMFLOAT3 m_Scale;
		std::string m_DudvMap;
		std::string m_NormalMap;
		uint32_t m_Width;
		uint32_t m_Height;
	};


}

#endif
