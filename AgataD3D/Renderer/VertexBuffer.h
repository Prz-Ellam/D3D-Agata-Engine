#ifndef AGATA_D11_VERTEX_BUFFER_H
#define AGATA_D11_VERTEX_BUFFER_H
#pragma once

#include <d3d11.h>
#include <vector>
#include "BufferLayout.h"
#include "IBindable.h"

namespace Agata {

	class VertexBuffer : public IBindable {
	public:
		VertexBuffer(const void* data, size_t size);
		//VertexBuffer(const VertexBuffer& rhs);
		VertexBuffer(VertexBuffer&& rhs) noexcept;
		virtual ~VertexBuffer();

		void SetLayout(const std::vector<BufferLayout>& bufferLayout);

		void Bind(uint32_t slot = 0u) const override;
	private:
		ID3D11Buffer* m_ID;
		ID3D11InputLayout* m_InputLayout;
		UINT m_Stride;
		UINT m_Offset;
	};

}

#endif
