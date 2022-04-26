#include "pch.h"
#include "Mesh.h"

namespace Agata {

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices) {

		m_VertexCount = vertices.size();
		m_VBO = std::make_shared<VertexBuffer>(vertices.data(), m_VertexCount * sizeof(Vertex));

		m_IndexCount = indices.size();
		m_IBO = std::make_shared<IndexBuffer>(indices.data(), m_IndexCount * sizeof(UINT));

		std::vector<BufferLayout> inputLayout;
		inputLayout.reserve(5);
		inputLayout.emplace_back(BufferLayout("POSITION", 0, DataType::Float3));
		inputLayout.emplace_back(BufferLayout("TEXCOORDS", 1, DataType::Float2));
		inputLayout.emplace_back(BufferLayout("NORMAL", 3, DataType::Float3));
		inputLayout.emplace_back(BufferLayout("TANGENT", 4, DataType::Float3));
		inputLayout.emplace_back(BufferLayout("BITANGENT", 5, DataType::Float3));

		m_VBO->SetLayout(inputLayout);

	}

	Mesh::Mesh(const std::vector<AnimVertex>& vertices, const std::vector<UINT>& indices) {

		m_VertexCount = vertices.size();
		m_VBO = std::make_shared<VertexBuffer>(vertices.data(), m_VertexCount * sizeof(AnimVertex));

		m_IndexCount = indices.size();
		m_IBO = std::make_shared<IndexBuffer>(indices.data(), m_IndexCount * sizeof(UINT));

		std::vector<BufferLayout> inputLayout;
		inputLayout.reserve(7);
		inputLayout.emplace_back(BufferLayout("POSITION", 0, DataType::Float3));
		inputLayout.emplace_back(BufferLayout("TEXCOORDS", 1, DataType::Float2));
		inputLayout.emplace_back(BufferLayout("NORMAL", 3, DataType::Float3));
		inputLayout.emplace_back(BufferLayout("TANGENT", 4, DataType::Float3));
		inputLayout.emplace_back(BufferLayout("BITANGENT", 5, DataType::Float3));
		inputLayout.emplace_back(BufferLayout("JOINT", 6, DataType::Float4));
		inputLayout.emplace_back(BufferLayout("WEIGHT", 7, DataType::Float4));

		m_VBO->SetLayout(inputLayout);

	}

	Mesh::Mesh(const std::vector<DX::XMFLOAT3>& vertices, const std::vector<UINT>& indices) {

		m_VertexCount = static_cast<uint32_t>(vertices.size());
		m_VBO = std::make_shared<VertexBuffer>(vertices.data(), m_VertexCount * sizeof(DX::XMFLOAT3));

		m_IndexCount = static_cast<uint32_t>(indices.size());
		m_IBO = std::make_shared<IndexBuffer>(indices.data(), m_IndexCount * sizeof(UINT));

		std::vector<BufferLayout> inputLayout;
		inputLayout.reserve(1);
		inputLayout.emplace_back(BufferLayout("POSITION", 0, DataType::Float3));
		//inputLayout.emplace_back(BufferLayout("TEXCOORDS", 1, DataType::Float2));

		m_VBO->SetLayout(inputLayout);

	}

	Mesh::~Mesh() {

	}

	std::shared_ptr<VertexBuffer> Mesh::GetVertexBuffer() const {

		return m_VBO;

	}

	std::shared_ptr<IndexBuffer>  Mesh::GetIndexBuffer() const {

		return m_IBO;

	}

	uint32_t Mesh::GetVertexCount() const {

		return m_VertexCount;

	}

	uint32_t Mesh::GetIndexCount() const {

		return m_IndexCount;

	}


}