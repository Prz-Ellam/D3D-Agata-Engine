#include "pch.h"
#include "ConstantBuffer.h"
#include "Renderer.h"

namespace Agata {

	ConstantBuffer::ConstantBuffer(const void* data, size_t size) : m_ByteSize(size) {

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = static_cast<UINT>(size);
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresourceData = { 0 };
		subresourceData.pSysMem = data;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr;
		hr = Renderer::GetDevice()->CreateBuffer(&bufferDesc, &subresourceData, &m_ID);
		if (FAILED(hr)) {
			OutputDebugString("No se pudo crear el Constant Buffer\n");
			// Error
		}

	}

	ConstantBuffer::~ConstantBuffer() {

		if (m_ID) {
			m_ID->Release();
		}

	}

	void ConstantBuffer::UpdateData(const void* data) {

		D3D11_MAPPED_SUBRESOURCE msr = { 0 };
		Renderer::GetDeviceContext()->Map(m_ID, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, data, m_ByteSize);
		Renderer::GetDeviceContext()->Unmap(m_ID, 0);

	}

	void ConstantBuffer::Bind() const {

		Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ID);

	}

}