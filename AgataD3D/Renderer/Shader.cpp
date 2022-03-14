#include "pch.h"
#include "Shader.h"
#include "Renderer.h"
#include <d3dcompiler.h>

namespace Agata {

	const Shader* Shader::s_ActualShader = nullptr;

	Shader::Shader(const std::string& vertexPath, const std::string& pixelPath) {

		HRESULT hr = 0;

		std::wstring path(vertexPath.begin(), vertexPath.end());
		hr = D3DReadFileToBlob(path.c_str(), &m_VertexShaderCode);

		if (FAILED(hr)) {
			// Error
		}

		hr = Renderer::GetDevice()->CreateVertexShader(m_VertexShaderCode->GetBufferPointer(),
			m_VertexShaderCode->GetBufferSize(), nullptr, &m_VertexShaderID);

		if (FAILED(hr)) {
			// Error
		}

		path = std::move(std::wstring(pixelPath.begin(), pixelPath.end()));
		hr = D3DReadFileToBlob(path.c_str(), &m_PixelShaderCode);

		if (FAILED(hr)) {
			// Error
		}

		hr = Renderer::GetDevice()->CreatePixelShader(m_PixelShaderCode->GetBufferPointer(),
			m_PixelShaderCode->GetBufferSize(), nullptr, &m_PixelShaderID);

		if (FAILED(hr)) {
			// Error
		}

	}

	Shader::~Shader() {

		if (m_VertexShaderCode) {
			m_VertexShaderCode->Release();
		}

		if (m_VertexShaderID) {
			m_VertexShaderID->Release();
		}

		if (m_PixelShaderCode) {
			m_PixelShaderCode->Release();
		}

		if (m_PixelShaderID) {
			m_PixelShaderID->Release();
		}

	}

	void Shader::Bind() const {

		s_ActualShader = this;
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShaderID, nullptr, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShaderID, nullptr, 0);

	}

	const Shader* Shader::GetActualShader() {

		return s_ActualShader;

	}

	const void* Shader::GetVSBuffer() const {

		return m_VertexShaderCode->GetBufferPointer();

	}

	size_t Shader::GetVSSize() const {

		return m_VertexShaderCode->GetBufferSize();

	}

}