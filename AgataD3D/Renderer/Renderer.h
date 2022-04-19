#ifndef AGATA_D11_RENDERER_H
#define AGATA_D11_RENDERER_H
#pragma once

#include "Window.h"
#include "Camera.h"

namespace Agata {

	class Mesh;

	enum class DepthFuncType {
		Lequal = D3D11_COMPARISON_LESS_EQUAL,
		Less = D3D11_COMPARISON_LESS
	};

	class Renderer {
	public:
		friend class Window;
		Renderer() = delete;
		Renderer(const Renderer& rhs) = delete;
		Renderer(Renderer&& rhs) = delete;
		virtual ~Renderer() = delete;

		static bool Init(std::unique_ptr<Window>& window);
		static void Release();

		static bool DepthFunc(DepthFuncType mode);

		static void Clear(FLOAT r, FLOAT g, FLOAT b, FLOAT a);

		static void DrawArrays(uint32_t vertexCount);
		static void DrawIndexes(uint32_t indexCount);
		static void DrawIndexes(Mesh* mesh);

		static void BeginScene(std::unique_ptr<Camera>& camera);

		static ID3D11Device* GetDevice() {
			return m_Device;
		}

		static ID3D11DeviceContext* GetDeviceContext() {
			return m_DeviceContext;
		}

		static DirectX::XMMATRIX GetProjection() {
			return s_Projection;
		}

		static DirectX::XMMATRIX GetView() {
			return s_View;
		}

		static DirectX::XMFLOAT3 GetPosition() {
			return s_Position;
		}

		static void SetDefaultFramebuffer() {
			m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
		}

		static ID3D11DepthStencilView* GetDepthStencilView() {
			return m_DepthStencilView;
		}

		static IDXGISwapChain* GetSwapChain() {
			return m_SwapChain;
		}

	private:
		static IDXGISwapChain* m_SwapChain;
		static ID3D11Device* m_Device;
		static ID3D11DeviceContext* m_DeviceContext;
		static ID3D11RenderTargetView* m_RenderTargetView;
		static ID3D11DepthStencilState* m_DepthStencilState;
		static ID3D11DepthStencilView* m_DepthStencilView;
		static ID3D11Texture2D* m_DepthStencil;
		static ID3D11RasterizerState* m_RasterizerState;
		static ID3D11BlendState* m_BlendState;

		static D3D11_DEPTH_STENCIL_DESC dsDesc;

		static DirectX::XMMATRIX s_View;
		static DirectX::XMMATRIX s_Projection;
		static DirectX::XMFLOAT3 s_Position;
		static DirectX::XMFLOAT3 s_Direction;
	};

}

#endif
