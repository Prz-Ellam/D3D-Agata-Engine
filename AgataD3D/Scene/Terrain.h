/*
    Terrain.h
	Proposito: Manejar recursos y dibujado del terreno del mundo
	@autor Eliam Perez
	Agata Engine Direct3D 11
*/

#ifndef AGATA_D11_TERRAIN_H
#define AGATA_D11_TERRAIN_H
#pragma once

#include "Loader.h"
#include "Mesh.h"
#include "Renderer.h"
#include "ConstantBuffer.h"
#include "Texture2D.h"
#include "Drawable.h"

namespace Agata {

	/*
	    Estructura para manejar el Constant Buffer del Terreno y los parametros que se
	    mandan al Shader
	*/
	HLSL struct TerrainBuffer {
		DX::XMMATRIX c_Model;
		DX::XMMATRIX c_View;
		DX::XMMATRIX c_Projection;
		DX::XMFLOAT4 c_CameraPosition;
		DX::XMFLOAT4 c_LightPosition;
		DX::XMFLOAT4 c_LightColour;
		float c_Tiling;
	};

	class Terrain : public Drawable {
	public:
		Terrain(const std::string& diffuseRed, const std::string& diffuseGreen, 
			const std::string& diffuseBlue, const std::string& textureBlack,
			const std::string& normalRed, const std::string& normalGreen, const std::string& normalBlue,
			const std::string& normalBlack, const std::string& blendMap, const std::string& heightMap, 
			const DX::XMFLOAT3& position,
			uint32_t height, uint32_t xSize, uint32_t zSize, float tilingFactor);
		Terrain(const Terrain& rhs) = default;
		Terrain(Terrain&& rhs) noexcept = default;
		virtual ~Terrain() = default;

		Terrain& operator=(const Terrain& rhs) = default;
		Terrain& operator=(Terrain&& rhs) noexcept = default;

		/*
		    Se obtiene la posicion en y del terreno en el mundo en base a su posicion 
			en x, z
			@param x posicion del mundo en x
			@param z posicion del mundo en z
			@return posicion del mundo en y
		*/
		float GetHeight(float x, float y);

		void OnRender() override;
	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<ConstantBuffer> m_CBO;
		TerrainBuffer m_Buffer;

		float m_TilingFactor;

		Texture2D m_TextureRed;
		Texture2D m_TextureGreen;
		Texture2D m_TextureBlue;
		Texture2D m_TextureBlack;

		Texture2D m_NormalRed;
		Texture2D m_NormalGreen;
		Texture2D m_NormalBlue;
		Texture2D m_NormalBlack;

		Texture2D m_BlendMap;
		TerrainProps m_Properties;
	};

	class TerrainBuilder {
	public:
		TerrainBuilder() = default;

		static TerrainBuilder GenerateParams();

		TerrainBuilder& RedTexture(const std::string& filePath);
		TerrainBuilder& GreenTexture(const std::string& filePath);
		TerrainBuilder& BlueTexture(const std::string& filePath);
		TerrainBuilder& BlackTexture(const std::string& filePath);

		TerrainBuilder& RedNormal(const std::string& filePath);
		TerrainBuilder& GreenNormal(const std::string& filePath);
		TerrainBuilder& BlueNormal(const std::string& filePath);
		TerrainBuilder& BlackNormal(const std::string& filePath);

		TerrainBuilder& Position(const DX::XMFLOAT3& position);
		TerrainBuilder& Rotation(const DX::XMFLOAT3& rotation);
		TerrainBuilder& Scale(const DX::XMFLOAT3& scale);

		TerrainBuilder& BlendMap(const std::string& filePath);
		TerrainBuilder& HeightMap(const std::string& filePath);

		TerrainBuilder& Width(float width);
		TerrainBuilder& Depth(float depth);
		TerrainBuilder& Height(float height);
		TerrainBuilder& TilingFactor(float tilingFactor);

		std::shared_ptr<Terrain> Build();
	private:
		std::string m_DiffuseTex;
		std::string m_NormalTex;
		std::string m_RedDiffuseTex;
		std::string m_RedNormalTex;
		std::string m_GreenDiffuseTex;
		std::string m_GreenNormalTex;
		std::string m_BlueDiffuseTex;
		std::string m_BlueNormalTex;
		std::string m_BlendMap;
		std::string m_HeightMap;
		DX::XMFLOAT3 m_Position;
		DX::XMFLOAT3 m_Rotation;
		DX::XMFLOAT3 m_Scale;
		float m_Height = 6.0f;
		float m_Width = 20.0f;
		float m_Depth = 20.0f;
		float m_TilingFactor = 1.0f;
	};

}

#endif // !AGATA_D11_TERRAIN_H
