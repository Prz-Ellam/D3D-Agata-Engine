#ifndef AGATA_D11_SHADER_H
#define AGATA_D11_SHADER_H
#pragma once

#include <string>
#include <d3d11.h>

namespace Agata {

	class Shader {
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		virtual ~Shader();

		void Bind() const;
		//void Unbind() const;

		static const Shader* GetActualShader();

		const void* GetVSBuffer() const;
		size_t GetVSSize() const;
	private:
		static const Shader* s_ActualShader;
		ID3D11VertexShader* m_VertexShaderID;
		ID3D11PixelShader* m_PixelShaderID;
		ID3DBlob* m_VertexShaderCode;
		ID3DBlob* m_PixelShaderCode;
	};

}


#endif
