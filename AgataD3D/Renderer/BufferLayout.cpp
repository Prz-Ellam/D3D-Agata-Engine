#include "pch.h"
#include "BufferLayout.h"

namespace Agata {

	BufferLayout::BufferLayout(const std::string& name, uint32_t index, DataType type) 
		: name(name), index(index), type(type), size(GetDataTypeSize(type)), count(GetDataCount(type)){

	}

	DXGI_FORMAT ConvertAgataToDXGIFormat(DataType type) {

		switch (type) {
			case DataType::Float:	return DXGI_FORMAT_R32_FLOAT;
			case DataType::Float2:	return DXGI_FORMAT_R32G32_FLOAT;
			case DataType::Float3:	return DXGI_FORMAT_R32G32B32_FLOAT;
			case DataType::Float4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case DataType::Int:     return DXGI_FORMAT_R32_SINT;
			case DataType::Int3:	return DXGI_FORMAT_R32G32B32_SINT;
		}

		return DXGI_FORMAT_UNKNOWN;

	}

	size_t BufferLayout::GetDataTypeSize(DataType type) {

		switch (type) {
			case DataType::Float:	return sizeof(float);
			case DataType::Float2:	return sizeof(float) * 2;
			case DataType::Float3:	return sizeof(float) * 3;
			case DataType::Float4:	return sizeof(float) * 4;
			case DataType::Int:     return sizeof(int);
			case DataType::Int3:	return sizeof(int) * 3;
		}

		return -1;

	}

	uint32_t BufferLayout::GetDataCount(DataType type) {

		switch (type) {
			case DataType::Float:	return 1;
			case DataType::Float2:	return 2;
			case DataType::Float3:	return 3;
			case DataType::Float4:	return 4;
			case DataType::Int:     return 1;
			case DataType::Int3:	return 3;
		}
		return 0;

	}

}