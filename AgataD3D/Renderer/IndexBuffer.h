#ifndef AGATA_D11_INDEX_BUFFER_H
#define AGATA_D11_INDEX_BUFFER_H
#pragma once

#include <d3d11.h>
#include "Renderer.h"

namespace Agata {

	class IndexBuffer {
	public:
		IndexBuffer(const void* data, size_t size);
		IndexBuffer(size_t size);
		virtual ~IndexBuffer();

		void Bind() const;
		void SendData(const void* data, size_t size, int64_t offset = 0ll);
	private:
		ID3D11Buffer* m_ID;
	};

}

#endif
