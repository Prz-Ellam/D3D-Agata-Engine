#ifndef AGATA_D11_TEXT_H
#define AGATA_D11_TEXT_H
#pragma once

#include <string>
#include <memory>
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Texture2D.h"
#include "Drawable.h"

namespace Agata {

	HLSL struct TextBuffer {
		DX::XMMATRIX c_Model;
	};

	class Text : public Drawable  {
	public:
		Text() = default;
		Text(const std::string& path);
		Text(const Text& other) = delete;
		Text(Text&& other) noexcept = delete;

		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) noexcept = delete;

		void DrawString(const std::string& text);
		void OnRender() override{ }
	private:
		void CreateCharacter(char character, int index);
		//std::shared_ptr<Mesh> m_Mesh;
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
		uint32_t m_IndexCount;
		uint32_t m_VertexSize;
		uint32_t m_IndicesSize;

		std::shared_ptr<ConstantBuffer> m_CBO;
		Texture2D m_Texture;
		TextBuffer m_Buffer;
	};

}

#endif
