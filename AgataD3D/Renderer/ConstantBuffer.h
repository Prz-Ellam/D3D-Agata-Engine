#ifndef AGATA_D11_CONSTANT_BUFFER_H
#define AGATA_D11_CONSTANT_BUFFER_H
#pragma once

#include <d3d11.h>

#define HLSL __declspec(align(16))

namespace Agata {

	class ConstantBuffer {
	public:
		ConstantBuffer(const void* data, size_t size);
		virtual ~ConstantBuffer();

		void UpdateData(const void* data);

		void Bind() const;
	private:
		ID3D11Buffer* m_ID;
		size_t m_ByteSize;
	};

}

#endif
