#ifndef AGATA_D11_BUFFER_LAYOUT
#define AGATA_D11_BUFFER_LAYOUT
#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

namespace Agata {

	enum class DataType {
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int3
	};


	DXGI_FORMAT ConvertAgataToDXGIFormat(DataType type);

	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 bitangent;
	};

	class BufferLayout {
	public:
		BufferLayout(const std::string& name, uint32_t index, DataType type);
		static size_t GetDataTypeSize(DataType type);
		static uint32_t GetDataCount(DataType type);
		std::string name;
		uint32_t index;
		DataType type;
		uint32_t count;
		size_t size;
	};

}

#endif
