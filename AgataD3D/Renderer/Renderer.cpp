#include "pch.h"
#include "Renderer.h"
#include "Mesh.h"
#include <array>

namespace Agata {

	IDXGISwapChain* Renderer::m_SwapChain = nullptr;
	ID3D11Device* Renderer::m_Device = nullptr;
	ID3D11DeviceContext* Renderer::m_DeviceContext = nullptr;
	ID3D11RenderTargetView* Renderer::m_RenderTargetView = nullptr;
	ID3D11DepthStencilState* Renderer::m_DepthStencilState = nullptr;
	ID3D11DepthStencilView* Renderer::m_DepthStencilView = nullptr;
	ID3D11Texture2D* Renderer::m_DepthStencil = nullptr;
	ID3D11RasterizerState* Renderer::m_RasterizerState = nullptr;
	ID3D11BlendState* Renderer::m_BlendState = nullptr;
	DirectX::XMMATRIX Renderer::s_View;
	DirectX::XMMATRIX Renderer::s_Projection;
	DirectX::XMFLOAT3 Renderer::s_Position;
	DirectX::XMFLOAT3 Renderer::s_Direction;
	D3D11_DEPTH_STENCIL_DESC Renderer::dsDesc = { 0 };

	bool Renderer::Init(std::unique_ptr<Window>& window) {

		std::array<D3D_DRIVER_TYPE, 3> driverTypes = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_SOFTWARE
		};

		std::array<D3D_FEATURE_LEVEL, 3> featureLevels = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		DXGI_SWAP_CHAIN_DESC scd = { 0 };
		scd.BufferCount = 1u;
		scd.BufferDesc.Width = 0u;
		scd.BufferDesc.Height = 0u;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = window->GetHandler();
		scd.Windowed = TRUE;
		//scd.SampleDesc.Count = 4u;
		scd.SampleDesc.Count = 1u;
		scd.SampleDesc.Quality = 0u;

		uint32_t creationFlags = 0;

#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT hr = 0;

		for (auto& driver : driverTypes) {

			hr = D3D11CreateDeviceAndSwapChain(0, driver, 0, creationFlags,
				featureLevels.data(), static_cast<UINT>(featureLevels.size()), D3D11_SDK_VERSION,
				&scd, &m_SwapChain, &m_Device, nullptr, &m_DeviceContext);

			if (SUCCEEDED(hr)) {
				break;
			}

		}

		if (FAILED(hr)) {
			OutputDebugString("No se pudo crear el Device y la Swap Chain\n");
			return false;
		}

		UINT a = 0;
		m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &a);

		ID3D11Texture2D* m_Resource = nullptr;
		hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_Resource));

		if (FAILED(hr)) {
			return false;
		}

		hr = m_Device->CreateRenderTargetView(m_Resource, nullptr, &m_RenderTargetView);

		if (FAILED(hr)) {
			return false;
		}

		if (m_Resource) {
			m_Resource->Release();
		}

		D3D11_VIEWPORT viewport = { 0 };
		viewport.Width = static_cast<float>(window->GetWidth());
		viewport.Height = static_cast<float>(window->GetHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		m_DeviceContext->RSSetViewports(1, &viewport);
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		hr = m_Device->CreateDepthStencilState(&dsDesc, &m_DepthStencilState);

		if (FAILED(hr)) {
			return false;
		}

		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1u);

		D3D11_TEXTURE2D_DESC stencilDesc = { 0 };
		stencilDesc.Width = static_cast<UINT>(window->GetWidth());
		stencilDesc.Height = static_cast<UINT>(window->GetHeight());
		stencilDesc.MipLevels = 1u;
		stencilDesc.ArraySize = 1u;
		stencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
		//stencilDesc.SampleDesc.Count = 4u;
		stencilDesc.SampleDesc.Count = 1u;
		stencilDesc.SampleDesc.Quality = 0u;
		stencilDesc.Usage = D3D11_USAGE_DEFAULT;
		stencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		hr = m_Device->CreateTexture2D(&stencilDesc, nullptr, &m_DepthStencil);

		if (FAILED(hr)) {
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		//dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		//dsvDesc.Texture2D.MipSlice = 1u;
		dsvDesc.Texture2D.MipSlice = 0u;

		hr = m_Device->CreateDepthStencilView(m_DepthStencil, &dsvDesc, &m_DepthStencilView);

		if (FAILED(hr)) {
			return false;
		}

		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);


		D3D11_BLEND_DESC blendDesc = {};
		auto& blendTarget = blendDesc.RenderTarget[0];
		blendTarget.BlendEnable = TRUE; // FALSE para no tener alpha blending
		blendTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendTarget.BlendOp = D3D11_BLEND_OP_ADD;
		blendTarget.SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
		blendTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // FALSE
		hr = m_Device->CreateBlendState(&blendDesc, &m_BlendState);

		if (FAILED(hr)) {
			return false;
		}

		m_DeviceContext->OMSetBlendState(m_BlendState, nullptr, 0xFFFFFFFF);


		D3D11_RASTERIZER_DESC rasterDesc = { };
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.MultisampleEnable = TRUE;
		rasterDesc.AntialiasedLineEnable = TRUE;
		hr = m_Device->CreateRasterizerState(&rasterDesc, &m_RasterizerState);

		if (FAILED(hr)) {
			return false;
		}

		m_DeviceContext->RSSetState(m_RasterizerState);


		return true;

	}

	bool Renderer::DepthFunc(DepthFuncType mode) {

		switch (mode) {
			case DepthFuncType::Less: {
				dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
				dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				break;
			}
			case DepthFuncType::Lequal: {
				dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
				dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				break;
			}
			default: {
				break;
			}
		}

		HRESULT hr = m_Device->CreateDepthStencilState(&dsDesc, &m_DepthStencilState);

		if (FAILED(hr)) {
			return false;
		}

	}

	void Renderer::Release() {

		if (m_SwapChain) {
			m_SwapChain->Release();
			m_SwapChain = nullptr;
		}

		if (m_Device) {
			m_Device->Release();
			m_Device = nullptr;
		}

		if (m_DeviceContext) {
			m_DeviceContext->Release();
			m_DeviceContext = nullptr;
		}

		if (m_RenderTargetView) {
			m_RenderTargetView->Release();
			m_RenderTargetView = nullptr;
		}

		if (m_DepthStencilState) {
			m_DepthStencilState->Release();
			m_DepthStencilState = nullptr;
		}

		if (m_DepthStencilView) {
			m_DepthStencilView->Release();
			m_DepthStencilView = nullptr;
		}

		if (m_DepthStencil) {
			m_DepthStencil->Release();
			m_DepthStencil = nullptr;
		}

		if (m_RasterizerState) {
			m_RasterizerState->Release();
			m_RasterizerState = nullptr;
		}

	}

	void Renderer::Clear(FLOAT r, FLOAT g, FLOAT b, FLOAT a) {

		FLOAT rgba[] = { r, g, b, a };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, rgba);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0u);

	}

	void Renderer::DrawArrays(uint32_t vertexCount) {

		m_DeviceContext->Draw(vertexCount, 0u);

	}

	void Renderer::DrawIndexes(uint32_t indexCount) {

		m_DeviceContext->DrawIndexed(indexCount, 0u, 0u);

	}

	void Renderer::DrawIndexes(Mesh* mesh) {

		mesh->GetVertexBuffer()->Bind();
		mesh->GetIndexBuffer()->Bind();

		m_DeviceContext->DrawIndexed(mesh->GetIndexCount(), 0u, 0u);

	}

	void Renderer::BeginScene(std::unique_ptr<Camera>& camera) {

		s_Projection = camera->GetProjection();
		s_View = camera->GetView();
		s_Position = camera->GetPosition();

	}

}