#ifndef AGATA_D11_MESH_H
#define AGATA_D11_MESH_H
#pragma once

#include <vector>
#include <memory>
#include <DirectXMath.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Vertex;

namespace DX = DirectX;

namespace Agata {

	class Mesh {
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices);
		Mesh(const std::vector<AnimVertex>& vertices, const std::vector<UINT>& indices);
		Mesh(const std::vector<DX::XMFLOAT3>& vertices, const std::vector<UINT>& indices);
		virtual ~Mesh();
		uint32_t GetVertexCount() const;
		uint32_t GetIndexCount() const;

		std::shared_ptr<VertexBuffer> GetVertexBuffer() const;
		std::shared_ptr<IndexBuffer> GetIndexBuffer() const;
	private:
		std::shared_ptr<VertexBuffer> m_VBO;
		std::shared_ptr<IndexBuffer> m_IBO;
		uint32_t m_VertexCount;
		uint32_t m_IndexCount;
	};

}

#endif // !AGATA_D11_MESH_H