#include "pch.h"
#include "Text.h"
#include "Loader.h"

#define MAX_CHARACTERS 30

namespace Agata {

	ID3D11SamplerState* Text::s_SamplerState = nullptr;
	bool Text::s_SamplerCreated;

	Text::Text(const std::string& path) :
		Drawable(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(1.0f * 4 / 7, 1.0f, 1.0f)), m_Texture(path) {

		//m_Mesh = Loader::Get().LoadQuad();
		m_VertexSize = 4 * sizeof(Vertex);
		m_IndicesSize = 6 * sizeof(UINT);

		m_VertexBuffer = new VertexBuffer(MAX_CHARACTERS * m_VertexSize);
		m_IndexBuffer = new IndexBuffer(MAX_CHARACTERS * m_IndicesSize);
		m_IndexCount = 0;
		
		m_CBO = std::make_shared<ConstantBuffer>(&m_Buffer, sizeof(TextBuffer));

		std::vector<BufferLayout> inputLayout;
		inputLayout.reserve(5);
		inputLayout.emplace_back(BufferLayout("POSITION", 0, DataType::Float3));
		inputLayout.emplace_back(BufferLayout("TEXCOORDS", 1, DataType::Float2));
		inputLayout.emplace_back(BufferLayout("NORMAL", 3, DataType::Float3));
		inputLayout.emplace_back(BufferLayout("TANGENT", 4, DataType::Float3));
		inputLayout.emplace_back(BufferLayout("BITANGENT", 5, DataType::Float3));

		m_VertexBuffer->SetLayout(inputLayout);

		CreateSamplerState();

	}

	void Text::DrawString(const std::string& text, const DirectX::XMFLOAT2 position, float scale) {

		std::vector<Vertex> verticesData;
		std::vector<UINT> indicesData;

		m_IndexCount = 0;
		int i = 0;
		for (auto& character : text) {
			
			auto [vertices, indices] = Loader::Get().LoadDynamicQuad();

			int ascii = static_cast<int>(character);

			float uStep = 1.0f / 32.0f; // Length of characters in image
			float vStep = 1.0f / 3.0f; 

			int row = 2 - ((ascii / 32) - 1);
			int column = ((ascii / 32.0f) - row - 1) * 32;

			float xOffset = uStep * column;
			float yOffset = vStep * row;

			vertices[0].pos.x += 0.1f * i;
			vertices[1].pos.x += 0.1f * i;
			vertices[2].pos.x += 0.1f * i;
			vertices[3].pos.x += 0.1f * i;

			vertices[0].uv = DirectX::XMFLOAT2(xOffset, yOffset);
			vertices[1].uv = DirectX::XMFLOAT2(xOffset + uStep, yOffset);
			vertices[2].uv = DirectX::XMFLOAT2(xOffset + uStep, yOffset + vStep);
			vertices[3].uv = DirectX::XMFLOAT2(xOffset, yOffset + vStep);

			for (uint32_t& index : indices) {
				index = index + (m_IndexCount / 6) * vertices.size();
			}

			for (auto& vertex : vertices) {
				verticesData.push_back(vertex);
			}

			for (auto& index : indices) {
				indicesData.push_back(index);
			}

			m_IndexCount += 6;
			i++;
		}

		m_VertexBuffer->SendData(verticesData.data(), verticesData.size() * sizeof(Vertex));
		m_IndexBuffer->SendData(indicesData.data(), indicesData.size() * sizeof(UINT));

		SetPosition(DirectX::XMFLOAT3(position.x, position.y, 0.0f));
		SetScale(DirectX::XMFLOAT3(1.0f * 4 / 7 * scale, 1.0f * scale, 1.0f));
		m_Buffer.c_Model = DX::XMMatrixTranspose(m_Transformation);
		m_CBO->Bind();
		m_CBO->UpdateData(&m_Buffer);

		m_Texture.Bind(0);

		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();
		Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &s_SamplerState);
		
		Renderer::GetDeviceContext()->DrawIndexed(m_IndexCount, 0u, 0u);
		
	}

	void Text::CreateCharacter(char character, int index) {

		auto [vertices, indices] = Loader::Get().LoadDynamicQuad();

		int ascii = static_cast<int>(character);

		float uStep = 1.0f / 32.0f; // Length of characters in image
		float vStep = 1.0f / 3.0f; 

		int row = 2 - ((ascii / 32) - 1);
		int column = ((ascii / 32.0f) - row - 1) * 32;


		float xOffset = uStep * column;
		float yOffset = vStep * row;

		vertices[0].uv = DirectX::XMFLOAT2(xOffset, yOffset);
		vertices[1].uv = DirectX::XMFLOAT2(xOffset + uStep, yOffset);
		vertices[2].uv = DirectX::XMFLOAT2(xOffset + uStep, yOffset + vStep);
		vertices[3].uv = DirectX::XMFLOAT2(xOffset, yOffset + vStep);


		//m_VertexBuffer->SendData(vertices.data(), vertices.size() * sizeof(Vertex), m_VertexSize * index);
		//m_IndexBuffer->SendData(indices.data(), indices.size() * sizeof(UINT), m_IndicesSize * index);
		//m_IndexCount = indices.size();

	}

	bool Text::CreateSamplerState() {

		if (!s_SamplerCreated) {

			D3D11_SAMPLER_DESC samplerDesc = {};
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			HRESULT hr = Renderer::GetDevice()->CreateSamplerState(&samplerDesc, &s_SamplerState);

			if (FAILED(hr)) {
				OutputDebugString("No se pudo crear el Sampler State\n");
				return false;
			}

			Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &s_SamplerState);
			s_SamplerCreated = true;

			}

			return true;

		}

	}
