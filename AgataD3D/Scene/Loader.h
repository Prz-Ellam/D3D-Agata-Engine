#ifndef AGATA_D11_LOADER_H
#define AGATA_D11_LOADER_H
#pragma once

#include <string>
#include "Mesh.h"
#include "BoxCollider.h"
#include <tuple>
#include <DirectXMath.h>
#undef min
#undef max
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Agata {

	struct TerrainParams {
		std::string HeightMap;
		uint32_t xSize;
		uint32_t zSize;
		uint32_t ySize;
	};

	struct TerrainProps {
		std::vector<float> Heights;
		uint32_t Width;
		float GridSpacingX;
		float GridSpacingZ;
	};

	struct SkeletalModelProps {
		aiScene Scene;
		std::vector<Mesh> Meshes;
		std::map<std::string, uint32_t> JointInfo;
		std::vector< std::pair<DirectX::XMMATRIX, DirectX::XMMATRIX>> JointTransformations;
		DirectX::XMMATRIX GlobalInverseTransformation;
		std::map<std::string, aiNodeAnim> AnimationInfo;
		float TicksPerSec;
		uint32_t JointCount = 0;
	};

	class Loader {
	public:
		Loader(const Loader& other) = delete;
		Loader(Loader&& other) noexcept = delete;
		~Loader() = default;

		Loader& operator=(const Loader& other) = delete;
		Loader& operator=(Loader&& other) noexcept = delete;

		static Loader& Get();

		std::shared_ptr<Mesh> LoadStaticModel(const std::string& path);
		std::shared_ptr<Mesh> LoadStaticModel(const std::string& filePath, BoxCollider& collider);
		std::pair<std::shared_ptr<Mesh>, TerrainProps> LoadTerrain(TerrainParams& terrainParams);
		std::shared_ptr<Mesh> LoadSkybox(float size);
		std::shared_ptr<Mesh> LoadQuad();
		std::tuple<std::vector<Vertex>, std::vector<UINT>> LoadDynamicQuad();
		std::shared_ptr<Mesh> LoadHorizontalQuad();

		DirectX::XMFLOAT3 GetNormals(int x, int y, int length, int width, int height, uint8_t* data);
		float GetHeight(int x, int z, int length, int width, int height, uint8_t* data);
	private:
		Loader() = default;
		static Loader s_Instance;
	};
}

#endif
