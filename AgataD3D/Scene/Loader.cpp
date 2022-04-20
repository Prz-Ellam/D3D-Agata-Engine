#include "pch.h"
#include "Loader.h"
#include <stb/stb_image.h>
#include "InputLayout.h"

namespace DX = DirectX;

namespace Agata {

	Loader Loader::s_Instance;

	Loader& Loader::Get() {

		return s_Instance;

	}

	std::shared_ptr<Mesh> Loader::LoadStaticModel(const std::string& path) {
		
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
			aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);;
		
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			return nullptr;
		}
		
		aiMesh* mesh = scene->mMeshes[0];

		std::vector<Vertex> vertices;
		vertices.reserve(mesh->mNumVertices);
		
		for (uint32_t i = 0; i < mesh->mNumVertices; i++) {

			aiVector3D& aiVertex = mesh->mVertices[i];
			aiVector3D& aiTexCoord = mesh->mTextureCoords[0][i];
			aiVector3D& aiNormal = mesh->mNormals[i];
			aiVector3D& aiTangent = mesh->mTangents[i];
			aiVector3D& aiBitangent = mesh->mBitangents[i];
			Vertex vertex;

			vertex.pos.x = aiVertex.x / 39.3701;
			vertex.pos.y = aiVertex.y / 39.3701;
			vertex.pos.z = aiVertex.z / 39.3701;
			vertex.uv.x = aiTexCoord.x;
			vertex.uv.y = aiTexCoord.y;
			vertex.normal.x = aiNormal.x;
			vertex.normal.y = aiNormal.y;
			vertex.normal.z = aiNormal.z;
			vertex.tangent.x = aiTangent.x;
			vertex.tangent.y = aiTangent.y;
			vertex.tangent.z = aiTangent.z;
			vertex.bitangent.x = aiBitangent.x;
			vertex.bitangent.y = aiBitangent.y;
			vertex.bitangent.z = aiBitangent.z;

			vertices.emplace_back(vertex);
		
		}
		
		std::vector<UINT> indices;
		indices.reserve(mesh->mNumFaces * 3ll);

		for (uint32_t i = 0; i < mesh->mNumFaces; i++) {

			aiFace& face = mesh->mFaces[i];
			indices.emplace_back(face.mIndices[0]);
			indices.emplace_back(face.mIndices[1]);
			indices.emplace_back(face.mIndices[2]);

		}
		
		return std::make_shared<Mesh>(vertices, indices);
	}

	std::shared_ptr<Mesh> Loader::LoadSkeletalModel(const std::string& path) {

		SkeletalModelProps props;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
			aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);;

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			return nullptr;
		}

		if (scene->mAnimations[0]->mTicksPerSecond != 0.0) {
			props.TicksPerSec = scene->mAnimations[0]->mTicksPerSecond;
		}
		else {
			props.TicksPerSec = 25.0f;
		}

		ProcessNode(scene, scene->mRootNode);

		const aiAnimation* animation = scene->mAnimations[0];
		for (int i = 0; i < animation->mNumChannels; i++) {
			const aiNodeAnim* nodeAnim = animation->mChannels[i];
			//animationInfo[nodeAnim->mNodeName.C_Str()] = nodeAnim;
		}

	}

	std::pair<std::shared_ptr<Mesh>, TerrainProps> Loader::LoadTerrain(TerrainParams& terrainParams) {

		TerrainProps terrainProps;

		int length, width, bpp;
		stbi_set_flip_vertically_on_load(true);
		uint8_t* buffer = stbi_load(terrainParams.HeightMap.c_str(), &length, &width, &bpp, 1);

		if (!buffer) {
			buffer = new uint8_t;
			*buffer = 0;
			length = width = 1;
		}

		std::vector<Vertex> vertices;
		vertices.resize(length * width);
		terrainProps.Heights.resize(length * width);
		std::vector<UINT> indices;
		indices.resize(6ll * (length - 1ll) * (width - 1ll));

		terrainProps.GridSpacingX = terrainParams.xSize / (static_cast<float>(length) - 1);
		terrainProps.GridSpacingZ = terrainParams.zSize / (static_cast<float>(width ) - 1);
		terrainProps.Width = width;

		// Load coordinates, normals and texture coordinates
		for (int i = 0, z = 0; z < width; z++) {
			for (int x = 0; x < length; x++) {

				vertices[i].pos.x = x * terrainProps.GridSpacingX;
				vertices[i].pos.y = GetHeight(x, z, length, width, terrainParams.ySize, buffer);
				terrainProps.Heights[i] = vertices[i].pos.y;
				vertices[i].pos.z = z * terrainProps.GridSpacingZ;

				vertices[i].normal = GetNormals(x, z, length, width, terrainParams.ySize, buffer);

				vertices[i].uv.x = static_cast<float>(x) / (float)(length - 1);
				vertices[i].uv.y = static_cast<float>(z) / (float)(width - 1);

				i++;
			}
		}

		// Tangentes y Bitangentes
		for (int z = 0; z < width - 1; z++) {
			for (int x = 0; x < length - 1; x++) {

				int i0 = z * width + x;
				int i1 = (z + 1) * width + x;
				int i2 = z * width + (x + 1);

				DX::XMVECTOR deltaPos1 = DX::XMVectorSubtract(
					DX::XMVectorSet(vertices[i1].pos.x, vertices[i1].pos.y, vertices[i1].pos.z, 0.0f), 
					DX::XMVectorSet(vertices[i0].pos.x, vertices[i0].pos.y, vertices[i0].pos.z, 0.0f)
				);

				DX::XMVECTOR deltaPos2 = DX::XMVectorSubtract(
					DX::XMVectorSet(vertices[i2].pos.x, vertices[i2].pos.y, vertices[i2].pos.z, 0.0f), 
					DX::XMVectorSet(vertices[i0].pos.x, vertices[i0].pos.y, vertices[i0].pos.z, 0.0f)
				);

				DX::XMVECTOR deltaUV1 = DX::XMVectorSubtract(DX::XMVectorSet(vertices[i1].uv.x, vertices[i1].uv.y,
					0.0f, 0.0f), DX::XMVectorSet(vertices[i0].uv.x, vertices[i0].uv.y, 0.0f, 0.0f));
				DX::XMVECTOR deltaUV2 = DX::XMVectorSubtract(DX::XMVectorSet(vertices[i2].uv.x, vertices[i2].uv.y,
					0.0f, 0.0f), DX::XMVectorSet(vertices[i0].uv.x, vertices[i0].uv.y, 0.0f, 0.0f));

				float r = 1.0f / (DX::XMVectorGetX(deltaUV1) * DX::XMVectorGetY(deltaUV2) - 
					DX::XMVectorGetY(deltaUV1) * DX::XMVectorGetX(deltaUV2));

				DX::XMVECTOR tangent = DX::XMVectorScale(
					DX::XMVectorSubtract(
						DX::XMVectorScale(deltaPos1, DX::XMVectorGetY(deltaUV2)), 
						DX::XMVectorScale(deltaPos2, DX::XMVectorGetY(deltaUV1))
					), 
				r);

				DX::XMVECTOR bitangent = DX::XMVectorScale(
					DX::XMVectorSubtract(
						DX::XMVectorScale(deltaPos2, DX::XMVectorGetX(deltaUV2)), 
						DX::XMVectorScale(deltaPos1, DX::XMVectorGetX(deltaUV1))
					), 
					r);

				DX::XMStoreFloat3(&vertices[i0].tangent, tangent);
				vertices[i1].tangent = vertices[i0].tangent;
				vertices[i2].tangent = vertices[i0].tangent;

				DX::XMStoreFloat3(&vertices[i0].bitangent, bitangent);
				vertices[i1].bitangent = vertices[i0].bitangent;
				vertices[i2].bitangent = vertices[i0].bitangent;

			}
		}

		// Load indices
		for (int i = 0, z = 0; z < width - 1; z++) {
			for (int x = 0; x < length - 1; x++) {
				indices[i++] = z * width + x;			// Top Left
				indices[i++] = (z + 1) * width + x;		// Bottom Left
				indices[i++] = z * width + x + 1;		// Top Right
				indices[i++] = z * width + x + 1;		// Top Right
				indices[i++] = (z + 1) * width + x;		// Bottom Left
				indices[i++] = (z + 1) * width + x + 1;	// Bottom Right
			}
		}

		return std::pair<std::shared_ptr<Mesh>, TerrainProps>(std::make_shared<Mesh>(vertices, indices), terrainProps);

	}

	DirectX::XMFLOAT3 Loader::GetNormals(int x, int y, int length, int width, int height, uint8_t* data) {

		float left = GetHeight(x - 1, y, length, width, height, data);
		float right = GetHeight(x + 1, y, length, width, height, data);
		float top = GetHeight(x, y + 1, length, width, height, data);
		float bottom = GetHeight(x, y - 1, length, width, height, data);
		DX::XMVECTOR normal = DirectX::XMVectorSet(left - right, 2.0f, bottom - top, 0.0f);
		DX::XMFLOAT3 result;
		DX::XMStoreFloat3(&result, DX::XMVector3Normalize(normal));
		return result;

	}

	float Loader::GetHeight(int x, int z, int length, int width, int height, uint8_t* data) {

		if (x < 0 || x > length || z < 0 || z > width) {
			return 0;
		}

		float result = data[x + z * width];
		result /= 255.0f;
		result *= height;
		return result;

	}

	std::shared_ptr<Mesh> Loader::LoadSkybox(float size) {

		std::vector<DX::XMFLOAT3> verticesAux(8);
		verticesAux[0] = DX::XMFLOAT3(-size, -size, size);
		verticesAux[1] = DX::XMFLOAT3(size, -size, size);
		verticesAux[2] = DX::XMFLOAT3(size, -size, -size);
		verticesAux[3] = DX::XMFLOAT3(-size, -size, -size);
		verticesAux[4] = DX::XMFLOAT3(-size, size, size);
		verticesAux[5] = DX::XMFLOAT3(size, size, size);
		verticesAux[6] = DX::XMFLOAT3(size, size, -size);
		verticesAux[7] = DX::XMFLOAT3(-size, size, -size);

		std::vector<uint32_t> indicesAux(36);
		indicesAux = { 1, 2, 6, 6, 5, 1, 0, 4, 7, 7, 3, 0, 4, 5, 6, 6, 7, 4,
			0, 3, 2, 2, 1, 0, 0, 1, 5, 5, 4, 0, 3, 7, 6, 6, 2, 3 };

		return std::make_shared<Mesh>(verticesAux, indicesAux);

	}

	std::shared_ptr<Mesh> Loader::LoadQuad() {

		std::vector<Vertex> vertices;
		vertices.resize(4);
		vertices[0].pos = DX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vertices[1].pos = DX::XMFLOAT3(1.0f, -1.0f, 0.0f);
		vertices[2].pos = DX::XMFLOAT3(1.0f, 1.0f, 0.0f);
		vertices[3].pos = DX::XMFLOAT3(-1.0f, 1.0f, 0.0f);

		vertices[0].uv =  DX::XMFLOAT2(0.0f, 0.0f);
		vertices[1].uv =  DX::XMFLOAT2(1.0f, 0.0f);
		vertices[2].uv =  DX::XMFLOAT2(1.0f, 1.0f);
		vertices[3].uv =  DX::XMFLOAT2(0.0f, 1.0f);

		vertices[0].normal = DX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[1].normal = DX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[2].normal = DX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[3].normal = DX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[0].tangent = DX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertices[1].tangent = DX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertices[2].tangent = DX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertices[3].tangent = DX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertices[0].bitangent = DX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertices[1].bitangent = DX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertices[2].bitangent = DX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertices[3].bitangent = DX::XMFLOAT3(0.0f, 0.0f, 1.0f);

		std::vector<UINT> indices;
		indices.resize(6);
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 3;
		indices[5] = 0;

		return std::make_shared<Mesh>(vertices, indices);

	}

	std::shared_ptr<Mesh> Loader::LoadHorizontalQuad() {

		std::vector<Vertex> vertices;
		vertices.resize(4);
		vertices[0].pos = DX::XMFLOAT3(-10.5f, 0.0f, -10.5f);
		vertices[1].pos = DX::XMFLOAT3(10.5f, 0.0f, -10.5f);
		vertices[2].pos = DX::XMFLOAT3(10.5f, 0.0f, 10.5f);
		vertices[3].pos = DX::XMFLOAT3(-10.5f, 0.0f, 10.5f);
		vertices[0].uv =  DX::XMFLOAT2(0.0f, 0.0f);
		vertices[1].uv =  DX::XMFLOAT2(1.0f, 0.0f);
		vertices[2].uv =  DX::XMFLOAT2(1.0f, 1.0f);
		vertices[3].uv =  DX::XMFLOAT2(0.0f, 1.0f);

		std::vector<UINT> indices;
		indices.resize(6);
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 3;
		indices[5] = 0;

		return std::make_shared<Mesh>(vertices, indices);

	}

	void Loader::ProcessNode(const aiScene* scene, aiNode* node) {

	}

}