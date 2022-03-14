#include "pch.h"
#include "IndexBuffer.h"

namespace Agata {

	IndexBuffer::IndexBuffer(const void* data, size_t size) {

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = static_cast<UINT>(size);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
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
			OutputDebugString("No se pudo crear el Index Buffer\n");
			// Error
		}

	}

	IndexBuffer::~IndexBuffer() {

		if (m_ID) {
			m_ID->Release();
		}

	}

	void IndexBuffer::Bind() const {

		Renderer::GetDeviceContext()->IASetIndexBuffer(m_ID, DXGI_FORMAT_R32_UINT, 0);

	}


}