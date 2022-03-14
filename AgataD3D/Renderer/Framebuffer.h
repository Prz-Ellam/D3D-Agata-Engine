#ifndef AGATA_D11_FRAMEBUFFER_H
#define AGATA_D11_FRAMEBUFFER_H
#pragma once

#include <d3d11.h>

namespace Agata {

	class Framebuffer {
	public:
		Framebuffer(uint32_t width, uint32_t height);

		void BindFrameBuffer();
		void UnbindFramebuffer();

		void BindTexture(uint32_t slot = 0);

		void Clear(FLOAT r, FLOAT g, FLOAT b, FLOAT a);

	private:
		ID3D11Texture2D* m_Buffer;
		ID3D11RenderTargetView* m_Framebuffer;
		ID3D11ShaderResourceView* m_SRV;
	};

}

#endif // !AGATA_D11_FRAMEBUFFER_H
