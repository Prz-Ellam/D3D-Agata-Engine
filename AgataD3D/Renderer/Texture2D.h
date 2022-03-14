#ifndef AGATA_D11_TEXTURE_2D_H
#define AGATA_D11_TEXTURE_2D_H
#pragma once

#include <string>
#include <d3d11.h>
#include "IBindable.h"

namespace Agata {

	enum class DefaultTexture {
		NormalMap,
		SpecularMap,
		DiffuseMap,
		DuDvMap
	};

	class Texture2D : public IBindable {
	public:
		Texture2D(const std::string& filePath);
		Texture2D(const std::string& filePath, DefaultTexture defaultTex);
		Texture2D(const Texture2D& other) = default;
		Texture2D(Texture2D&& other) noexcept = default;
		virtual ~Texture2D();

		void Bind(uint32_t slot = 0u) const override;

		static ID3D11SamplerState** GetSampler() {
			return &s_SamplerState;
		}
	private:
		ID3D11Texture2D* m_ID;
		ID3D11ShaderResourceView* m_SRV;
		static ID3D11SamplerState* s_SamplerState;
		static bool s_SamplerCreated;
		int m_Width, m_Height, m_BPP;
		bool CreateTextureFromBuffer(uint8_t* buffer);
		bool CreateTextureFromDefault(DefaultTexture defaultTex);
		bool CreateSampler();
	};

}

#endif
