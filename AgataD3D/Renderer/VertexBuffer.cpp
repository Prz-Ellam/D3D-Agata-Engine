#include "pch.h"
#include "VertexBuffer.h"
#include "Renderer.h"
#include "Shader.h"

namespace Agata {

	VertexBuffer::VertexBuffer(const void* data, size_t size) : m_InputLayout(nullptr),
	m_Stride(0), m_Offset(0) {

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = static_cast<UINT>(size);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresourceData = { 0 };
		subresourceData.pSysMem = data;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr = 0;
		hr = Renderer::GetDevice()->CreateBuffer(&bufferDesc, &subresourceData, &m_ID);

		if (FAILED(hr)) {
			OutputDebugString("No se pudo crear el Vertex Buffer\n");
		}

	}

	//VertexBuffer::VertexBuffer(const VertexBuffer& rhs) {
	//
	//
	//}

	VertexBuffer::VertexBuffer(VertexBuffer&& rhs) noexcept {

		m_ID = rhs.m_ID;
		m_InputLayout = rhs.m_InputLayout;
		m_Offset = rhs.m_Offset;
		m_Stride = rhs.m_Stride;

		rhs.m_ID = nullptr;
		rhs.m_InputLayout = nullptr;
	
	}

	VertexBuffer::~VertexBuffer() {

		if (m_ID) {
			m_ID->Release();
		}

		if (m_InputLayout) {
			m_InputLayout->Release();
		}

	}

	void VertexBuffer::SetLayout(const std::vector<BufferLayout>& bufferLayout) {

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
		inputLayout.reserve(bufferLayout.size());

		m_Stride = 0;
		for (const BufferLayout& element : bufferLayout) {

			inputLayout.push_back(
				{ element.name.c_str(), 0, ConvertAgataToDXGIFormat(element.type), 0,
				D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

			m_Stride += element.size;

		}

		HRESULT hr = Renderer::GetDevice()->CreateInputLayout(inputLayout.data(), inputLayout.size(),
			Shader::GetActualShader()->GetVSBuffer(), Shader::GetActualShader()->GetVSSize(),
			&m_InputLayout);

		if (FAILED(hr)) {
			OutputDebugString("No se pudo crear el Input Layout\n");
			return;
		}

	}

	void VertexBuffer::Bind(uint32_t slot) const {

		// Advertencia, stride u offset es 0 
		Renderer::GetDeviceContext()->IASetVertexBuffers(slot, 1u, &m_ID, &m_Stride, &m_Offset);
		Renderer::GetDeviceContext()->IASetInputLayout(m_InputLayout);

	}

}