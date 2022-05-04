#include "pch.h"
#include "Framebuffer.h"
#include "Renderer.h"

namespace Agata {

	Framebuffer::Framebuffer(uint32_t width, uint32_t height) {

		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT hr;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1u;
		textureDesc.ArraySize = 1u;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//textureDesc.SampleDesc.Count = 4u;
		textureDesc.SampleDesc.Count = 1u;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		hr = Renderer::GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_Buffer);
		if (FAILED(hr))
		{
			return;
		}

		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		hr = Renderer::GetDevice()->CreateRenderTargetView(m_Buffer, &renderTargetViewDesc, &m_Framebuffer);

		if (FAILED(hr)) {
			return;
		}

		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		hr = Renderer::GetDevice()->CreateShaderResourceView(m_Buffer, &shaderResourceViewDesc, &m_SRV);

		if (FAILED(hr)) {
			return;
		}

		CreateDepthStencilView(width, height);

	}


	void Framebuffer::BindFrameBuffer() {

		Renderer::GetDeviceContext()->OMSetRenderTargets(1, &m_Framebuffer, Renderer::GetDepthStencilView());

	}

	void Framebuffer::UnbindFramebuffer() {

		Renderer::SetDefaultFramebuffer();

	}

	void Framebuffer::Clear(FLOAT r, FLOAT g, FLOAT b, FLOAT a) {

		FLOAT rgba[] = { r, g, b, a };
		Renderer::GetDeviceContext()->ClearRenderTargetView(m_Framebuffer, rgba);
		Renderer::GetDeviceContext()->ClearDepthStencilView(Renderer::GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

	}

	void Framebuffer::BindTexture(uint32_t slot) {

		Renderer::GetDeviceContext()->PSSetShaderResources(slot, 1u, &m_SRV);

	}

	bool Framebuffer::CreateDepthStencilView(int width, int height) {

		HRESULT hr;

		D3D11_TEXTURE2D_DESC stencilDesc = { 0 };
		stencilDesc.Width = 1280;
		stencilDesc.Height = 720;
		stencilDesc.MipLevels = 1u;
		stencilDesc.ArraySize = 1u;
		stencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
		stencilDesc.SampleDesc.Count = 1u;
		stencilDesc.SampleDesc.Quality = 0u;
		stencilDesc.Usage = D3D11_USAGE_DEFAULT;
		stencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		hr = Renderer::GetDevice()->CreateTexture2D(&stencilDesc, nullptr, &m_DepthStencil);

		if (FAILED(hr)) {
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		//dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 1u;
		//dsvDesc.Texture2D.MipSlice = 0u;

		hr = Renderer::GetDevice()->CreateDepthStencilView(m_DepthStencil, &dsvDesc, &m_DepthStencilView);

		if (FAILED(hr)) {
			return false;
		}

	}

}