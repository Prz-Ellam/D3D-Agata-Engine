#include "pch.h"
#include "Terrain.h"
#include "MathUtils.h"

namespace Agata {

	Terrain::Terrain(const std::string& textureRed, const std::string& textureGreen,
		const std::string& textureBlue, const std::string& textureBlack,
		const std::string& redNormal, const std::string& greenNormal, const std::string& blueNormal,
		const std::string& blackNormal, const std::string& blendMap,
		const std::string& heightMap, const DX::XMFLOAT3& position,
		uint32_t ySize, uint32_t xSize, uint32_t zSize, float tilingFactor)
		: m_TextureRed(textureRed), m_TextureGreen(textureGreen), m_TextureBlue(textureBlue),
		m_TextureBlack(textureBlack), m_BlendMap(blendMap), m_TilingFactor(tilingFactor),
		Drawable(position, position, position), m_NormalRed(redNormal), m_NormalGreen(greenNormal),
		m_NormalBlue(blueNormal), m_NormalBlack(blackNormal) {

		TerrainParams terrainParams;
		terrainParams.xSize = xSize;
		terrainParams.ySize = ySize;
		terrainParams.zSize = zSize;
		terrainParams.HeightMap = heightMap;

		auto [mesh, properties] = Loader::Get().LoadTerrain(terrainParams);
		m_Mesh.swap(mesh);
		m_Properties = std::move(properties);

		m_CBO = std::make_shared<ConstantBuffer>(&m_Buffer, sizeof(m_Buffer));

	}

	float Terrain::GetHeight(float x, float z) {

		// Se resta la posicion del parametro por su posicion actual para obtener la 
		// posicion relativa en 0,0 y simplificar calculos
		float terrainX = x - m_Position.x;
		float terrainZ = z - m_Position.z;

		// Se obtiene el grid especifico en el que se encuentra el punto de la posicion
		int gridX = (static_cast<int>(terrainX / m_Properties.GridSpacingX)) + 1;
		int gridZ = (static_cast<int>(terrainZ / m_Properties.GridSpacingZ)) + 1;

		// Se verifica que dicho grid forme parte del terreno, de lo contrario se 
		// devuelve 0.0f
		if (gridX < 0 || gridZ < 0 || gridX > std::sqrt(m_Properties.Heights.size()) - 1 || 
			gridZ > std::sqrt(m_Properties.Heights.size()) - 1) {
			return 0.0f;
		}

		// Se obtiene la posicion especifica en el grid
		float xCoord = (terrainX / m_Properties.GridSpacingX + 1) - gridX;
		float zCoord = (terrainZ / m_Properties.GridSpacingZ + 1) - gridZ;

		/* 
		    Cada grid se divide en dos triangulos, dependiendo el triangulo donde se
		    encuentra la posicion, hará una operacion distinta
		    Se realiza una operacion de coordenadas baricentricas para encontrar con 
			mayor exactitud la altura dentro del triangulo
		*/
		float height;
		if (xCoord <= (1 - zCoord)) {
			height = MathUtils::BaryCentricCoordinatesY(
				DX::XMFLOAT3(0, m_Properties.Heights[gridX + gridZ * m_Properties.Width], 0),
				DX::XMFLOAT3(1, m_Properties.Heights[(gridX + 1ll) + gridZ * m_Properties.Width], 0),
				DX::XMFLOAT3(0, m_Properties.Heights[gridX + (gridZ + 1ll) * m_Properties.Width], 1),
				DX::XMFLOAT2(xCoord, zCoord));
		}
		else {
			height = MathUtils::BaryCentricCoordinatesY(
				DX::XMFLOAT3(1, m_Properties.Heights[(gridX + 1ll) + gridZ * m_Properties.Width], 0),
				DX::XMFLOAT3(1, m_Properties.Heights[(gridX + 1ll) + (gridZ + 1ll) * m_Properties.Width], 1),
				DX::XMFLOAT3(0, m_Properties.Heights[gridX + (gridZ + 1ll) * m_Properties.Width], 1),
				DX::XMFLOAT2(xCoord, zCoord));
		}

		return height;

	}

	void Terrain::OnRender() {

		// Se actualizan los parametros del Constant Buffer
		m_Buffer.c_Model = DX::XMMatrixTranspose(DX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z));
		m_Buffer.c_View = DX::XMMatrixTranspose(Renderer::GetView());
		m_Buffer.c_Projection = DX::XMMatrixTranspose(Renderer::GetProjection());
		DX::XMFLOAT3 v =  Renderer::GetPosition();
		m_Buffer.c_CameraPosition = DX::XMFLOAT4(v.x, v.y, v.z, 1.0f);
		m_Buffer.c_LightPosition = Renderer::GetLight()->GetPosition();
		m_Buffer.c_LightColour = Renderer::GetLight()->GetColour();
		m_Buffer.c_Tiling = m_TilingFactor;
		m_CBO->Bind();
		m_CBO->UpdateData(&m_Buffer);

		// Se activan las texturas del terreno
		m_TextureRed.Bind(0);
		m_TextureGreen.Bind(1);
		m_TextureBlue.Bind(2);
		m_TextureBlack.Bind(3);
		m_BlendMap.Bind(4);

		m_NormalRed.Bind(5);
		m_NormalGreen.Bind(6);
		m_NormalBlue.Bind(7);
		m_NormalBlack.Bind(8);

		// Se manda a dibujar la malla del terreno
		Renderer::DrawIndexes(m_Mesh.get());

	}


	TerrainBuilder TerrainBuilder::GenerateParams() {

		return TerrainBuilder();

	}

	TerrainBuilder& TerrainBuilder::RedTexture(const std::string& filePath) {

		m_RedDiffuseTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::GreenTexture(const std::string& filePath) {

		m_GreenDiffuseTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::BlueTexture(const std::string& filePath) {

		m_BlueDiffuseTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::BlackTexture(const std::string& filePath) {

		m_DiffuseTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::Position(const DX::XMFLOAT3& position) {

		m_Position = position;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::Rotation(const DX::XMFLOAT3& rotation) {

		m_Rotation = rotation;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::Scale(const DX::XMFLOAT3& scale) {

		m_Scale = scale;
		return *this;

	}


	TerrainBuilder& TerrainBuilder::BlendMap(const std::string& filePath) {

		m_BlendMap = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::HeightMap(const std::string& filePath) {

		m_HeightMap = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::RedNormal(const std::string& filePath) {

		m_RedNormalTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::GreenNormal(const std::string& filePath) {

		m_GreenNormalTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::BlueNormal(const std::string& filePath) {

		m_BlueNormalTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::BlackNormal(const std::string& filePath) {

		m_NormalTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::Width(float width) {

		m_Width = width;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::Depth(float depth) {

		m_Depth = depth;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::Height(float height) {

		m_Height = height;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::TilingFactor(float tilingFactor) {

		m_TilingFactor = tilingFactor;
		return *this;

	}

	std::shared_ptr<Terrain> TerrainBuilder::Build() {

		return std::make_shared<Terrain>(m_RedDiffuseTex, m_GreenDiffuseTex, m_BlueDiffuseTex, 
			m_DiffuseTex, m_RedNormalTex, m_GreenNormalTex, m_BlueNormalTex, m_NormalTex,
			m_BlendMap, m_HeightMap, m_Position, m_Height, m_Width, m_Depth, m_TilingFactor);

	}

}