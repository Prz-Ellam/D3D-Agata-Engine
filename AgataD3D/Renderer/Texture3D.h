#ifndef AGATA_D11_TEXTURE_3D_H
#define AGATA_D11_TEXTURE_3D_H
#pragma once

#include <string>
#include <d3d11.h>
#include "IBindable.h"

namespace Agata {

	struct SkyboxTexture {
		std::string Right;
		std::string Left;
		std::string Top;
		std::string Bottom;
		std::string Front;
		std::string Back;
	};

	class Texture3D : public IBindable {
	public:
		Texture3D(const SkyboxTexture& paths);
		Texture3D(const Texture3D& other) = default;
		Texture3D(Texture3D&& other) noexcept = default;
		virtual ~Texture3D();

		void Bind(uint32_t slot = 0u) const override;
	private:
		ID3D11Texture2D* m_ID;
		ID3D11ShaderResourceView* m_SRV;
		static ID3D11SamplerState* s_SamplerState;
		static bool s_SamplerCreated;
		int m_Width, m_Height, m_BPP;
	};

}


#endif
