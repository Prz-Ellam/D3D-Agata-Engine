#include "pch.h"
#include "SkeletalModel.h"
#include "Loader.h"
#include "MathUtils.h"

namespace Agata {

	SkeletalModel::SkeletalModel(const std::string& filePath, const std::string& diffuseTex,
		const std::string& specularTex, const std::string& normalTex,
		const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation,
		const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT4& ambientMaterial,
		const DirectX::XMFLOAT4& diffuseMaterial, const DirectX::XMFLOAT4& specularMaterial,
		float shininessMaterial) : Drawable(position, rotation, scale) {

		//m_Properties = Loader::Get().LoadSkeletalModel(filePath);

		LoadSkeletalModel(filePath);

		m_CBO = std::make_shared<ConstantBuffer>(&m_Buffer, sizeof(m_Buffer));
		m_CBO2 = std::make_shared<ConstantBuffer>(&m_Buffer2, sizeof(m_Buffer2));
		m_Material = std::make_shared<Material>(diffuseTex, normalTex, specularTex, ambientMaterial, 
			diffuseMaterial, specularMaterial, shininessMaterial);

	}

	std::shared_ptr<Material>& SkeletalModel::GetMaterial() {

		return m_Material;
	}

	void SkeletalModel::LoadSkeletalModel(const std::string& path) {

		m_Scene = m_Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

		if (!m_Scene || !m_Scene->mRootNode || m_Scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
			return;
		}

		m_GlobalInverseTransformation = MathUtils::AssimpToDXMatrix(m_Scene->mRootNode->mTransformation);
		m_GlobalInverseTransformation = DirectX::XMMatrixInverse(nullptr, m_GlobalInverseTransformation);

		if (m_Scene->mAnimations[0]->mTicksPerSecond != 0.0) {
			m_TicksPerSec = m_Scene->mAnimations[0]->mTicksPerSecond;
		}
		else {
			m_TicksPerSec = 25.0f;
		}

		ProcessNode(m_Scene, m_Scene->mRootNode);

		/*
		Load the aiNodeAnim that contains all keyframes of a Joint
		*/
		const aiAnimation* animation = m_Scene->mAnimations[0];
		for (int i = 0; i < animation->mNumChannels; i++) {
			const aiNodeAnim* nodeAnim = animation->mChannels[i];
			m_AnimationInfo[nodeAnim->mNodeName.C_Str()] = nodeAnim;
		}

	}

	void SkeletalModel::ProcessNode(const aiScene* const scene, aiNode* node) {

		for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
			aiMesh* ai_mesh = scene->mMeshes[i];
			std::vector<AnimVertex> vertices;
			std::vector<uint32_t>indices;
			ProcessMesh(ai_mesh, scene, vertices, indices);
			m_Meshes.push_back(Mesh(vertices, indices));
		}

	}

	void SkeletalModel::ProcessMesh(aiMesh* mesh, const aiScene* scene, 
		std::vector<AnimVertex>& vertices, std::vector<uint32_t>& indices) {

		vertices = {};
		indices = {};
		vertices.resize(mesh->mNumVertices);
		indices.reserve(mesh->mNumFaces * 3ll);

		/*
		Load all vertex data
		*/
		for (uint32_t i = 0; i < mesh->mNumVertices; i++) {

			DirectX::XMFLOAT3 v;
			v.x = mesh->mVertices[i].x;
			v.y = mesh->mVertices[i].y;
			v.z = mesh->mVertices[i].z;
			vertices[i].pos = v;;

			DirectX::XMFLOAT2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			vertices[i].uv = uv;

			DirectX::XMFLOAT3 norm;
			norm.x = mesh->mNormals[i].x;
			norm.y = mesh->mNormals[i].y;
			norm.z = mesh->mNormals[i].z;
			vertices[i].normal = norm;

			DirectX::XMFLOAT3 tan;
			tan.x = mesh->mTangents[i].x;
			tan.y = mesh->mTangents[i].y;
			tan.z = mesh->mTangents[i].z;
			vertices[i].tangent = tan;

			DirectX::XMFLOAT3 bi;
			bi.x = mesh->mBitangents[i].x;
			bi.y = mesh->mBitangents[i].y;
			bi.z = mesh->mBitangents[i].z;
			vertices[i].bitangent = bi;

		}

		/*
		Load indices
		*/
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		/*
		Load Joints
		*/
		std::vector<uint8_t> jointPos(vertices.size());
		for (uint32_t i = 0; i < mesh->mNumBones; i++) {

			uint32_t jointIndex = 0;
			aiBone* joint = mesh->mBones[i];
			std::string jointName = joint->mName.C_Str();

			if (m_JointInfo.find(jointName) == m_JointInfo.end()) {

				jointIndex = m_JointCount;
				m_JointCount++;

				DirectX::XMMATRIX offsetMatrix = MathUtils::AssimpToDXMatrix(joint->mOffsetMatrix);
				m_JointTransformations.push_back(std::make_pair(offsetMatrix, DirectX::XMMatrixIdentity()));

				m_JointInfo[jointName] = jointIndex;

			}
			else {
				jointIndex = m_JointInfo[jointName];
			}

			for (int j = 0; j < joint->mNumWeights; j++) {

				uint32_t vertexID = joint->mWeights[j].mVertexId;
				float weight = joint->mWeights[j].mWeight;

				if (jointPos[vertexID] < 4) {

					switch (jointPos[vertexID]) {
					case 0:
						vertices[vertexID].joints.x = jointIndex;
						vertices[vertexID].weights.x = weight;
						break;
					case 1:
						vertices[vertexID].joints.y = jointIndex;
						vertices[vertexID].weights.y = weight;
						break;
					case 2:
						vertices[vertexID].joints.z = jointIndex;
						vertices[vertexID].weights.z = weight;
						break;
					case 3:
						vertices[vertexID].joints.w = jointIndex;
						vertices[vertexID].weights.w = weight;
						break;
					}

					jointPos[vertexID]++;
				}
			}
		}

	}

	void SkeletalModel::LoadJointsTransforms(float timeStamp, std::vector<DirectX::XMMATRIX>& transformations) {

		float timeTicks = timeStamp * m_TicksPerSec;
		float animationTime = fmod(timeTicks, m_Scene->mAnimations[0]->mDuration);
		FindTransformations(animationTime, m_Scene->mRootNode);

		transformations.clear();
		transformations.resize(m_JointCount);
		for (uint32_t i = 0; i < m_JointCount; i++) {
			transformations[i] = m_JointTransformations[i].second;  // first is offset, seconds is all transformation
		}

	}
	void SkeletalModel::FindTransformations(float timeStamp, const aiNode* node,
		const DirectX::XMMATRIX parentTransformation) {

		std::string nodeName(node->mName.data);
		DirectX::XMMATRIX nodeTransform = MathUtils::AssimpToDXMatrix(node->mTransformation);

		auto it = m_AnimationInfo.find(nodeName);
		const aiNodeAnim* nodeAnim;
		if (it != m_AnimationInfo.end()) {

			nodeAnim = it->second;
			DirectX::XMVECTOR lerpPos = LerpPosition(timeStamp, nodeAnim);
			DirectX::XMVECTOR lerpRot = LerpRotation(timeStamp, nodeAnim);
			DirectX::XMVECTOR lerpSca = LerpScale(timeStamp, nodeAnim);

			nodeTransform = MathUtils::TransformationMatrix(lerpPos, lerpRot, lerpSca);

		}

		DirectX::XMMATRIX globalTransform = nodeTransform * parentTransformation;

		if (m_JointInfo.find(nodeName) != m_JointInfo.end()) {
			uint32_t jointID = m_JointInfo[nodeName];
			m_JointTransformations[jointID].second =  m_JointTransformations[jointID].first * 
				globalTransform * m_GlobalInverseTransformation;
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++) {
			FindTransformations(timeStamp, node->mChildren[i], globalTransform);
		}

	}

	DirectX::XMVECTOR SkeletalModel::LerpPosition(float animationTime, const aiNodeAnim* node) {

		if (node->mNumPositionKeys == 1) {
			return MathUtils::AssimpToDXVector(node->mPositionKeys[0].mValue);
		}

		int positionIndex = 0, nextPosition;
		for (uint32_t i = 0; i < node->mNumPositionKeys - 1; i++) {
			if (animationTime < (float)node->mPositionKeys[i + 1].mTime) {
				positionIndex = i;
				break;
			}
		}

		nextPosition = positionIndex + 1;
		float dt = (float)(node->mPositionKeys[nextPosition].mTime - node->mPositionKeys[positionIndex].mTime);
		float factor = (animationTime - (float)node->mPositionKeys[positionIndex].mTime) / dt;
		DirectX::XMVECTOR start = MathUtils::AssimpToDXVector(node->mPositionKeys[positionIndex].mValue);
		DirectX::XMVECTOR end = MathUtils::AssimpToDXVector(node->mPositionKeys[nextPosition].mValue);
		DirectX::XMVECTOR delta = DirectX::XMVectorSubtract(end, start);

		return DirectX::XMVectorAdd(start, DirectX::XMVectorScale(delta, factor));

	}

	DirectX::XMVECTOR SkeletalModel::LerpRotation(float animationTime, const aiNodeAnim* node) {
		
		if (node->mNumRotationKeys == 1) {
			return MathUtils::AssimpToDXQuat(node->mRotationKeys[0].mValue);
		}

		int positionIndex = 0, nextPosition;
		for (uint32_t i = 0; i < node->mNumRotationKeys - 1; i++) {
			if (animationTime < (float)node->mRotationKeys[i + 1].mTime) {
				positionIndex = i;
				break;
			}
		}

		nextPosition = positionIndex + 1;
		float dt = (float)(node->mRotationKeys[nextPosition].mTime - node->mRotationKeys[positionIndex].mTime);
		float factor = (animationTime - (float)node->mRotationKeys[positionIndex].mTime) / dt;
		DirectX::XMVECTOR start = MathUtils::AssimpToDXQuat(node->mRotationKeys[positionIndex].mValue);
		DirectX::XMVECTOR end = MathUtils::AssimpToDXQuat(node->mRotationKeys[nextPosition].mValue);

		start = DirectX::XMQuaternionNormalize(start);
		end = DirectX::XMQuaternionNormalize(end);

		DirectX::XMVECTOR result = DirectX::XMVECTOR();

		DirectX::XMVectorGetX(DirectX::XMQuaternionDot(start, end));

		float dot = DirectX::XMVectorGetX(DirectX::XMQuaternionDot(start, end));
		float oneMinusFactor = 1.0f - factor;

		if (dot < 0.0f) {
			result = DirectX::XMVectorSetX(result, 
				oneMinusFactor * DirectX::XMVectorGetX(start) + factor * -DirectX::XMVectorGetX(end));
			result = DirectX::XMVectorSetY(result, 
				oneMinusFactor * DirectX::XMVectorGetY(start) + factor * -DirectX::XMVectorGetY(end));
			result = DirectX::XMVectorSetZ(result, 
				oneMinusFactor * DirectX::XMVectorGetZ(start) + factor * -DirectX::XMVectorGetZ(end));
			result = DirectX::XMVectorSetW(result, 
				oneMinusFactor * DirectX::XMVectorGetW(start) + factor * -DirectX::XMVectorGetW(end));
		}
		else {
			result = DirectX::XMVectorSetX(result, 
				oneMinusFactor * DirectX::XMVectorGetX(start) + factor * DirectX::XMVectorGetX(end));
			result = DirectX::XMVectorSetY(result, 
				oneMinusFactor * DirectX::XMVectorGetY(start) + factor * DirectX::XMVectorGetY(end));
			result = DirectX::XMVectorSetZ(result, 
				oneMinusFactor * DirectX::XMVectorGetZ(start) + factor * DirectX::XMVectorGetZ(end));
			result = DirectX::XMVectorSetW(result, 
				oneMinusFactor * DirectX::XMVectorGetW(start) + factor * DirectX::XMVectorGetW(end));
		}

		return DirectX::XMQuaternionNormalize(result);

	}

	DirectX::XMVECTOR SkeletalModel::LerpScale(float animationTime, const aiNodeAnim* node) {
		
		if (node->mNumScalingKeys == 1) {
			return MathUtils::AssimpToDXVector(node->mScalingKeys[0].mValue);
		}

		int positionIndex = 0, nextPosition;
		for (uint32_t i = 0; i < node->mNumScalingKeys - 1; i++) {
			if (animationTime < (float)node->mScalingKeys[i + 1].mTime) {
				positionIndex = i;
				break;
			}
		}

		nextPosition = positionIndex + 1;
		float dt = (float)(node->mScalingKeys[nextPosition].mTime - node->mScalingKeys[positionIndex].mTime);
		float factor = (animationTime - (float)node->mScalingKeys[positionIndex].mTime) / dt;
		DirectX::XMVECTOR start = MathUtils::AssimpToDXVector(node->mScalingKeys[positionIndex].mValue);
		DirectX::XMVECTOR end = MathUtils::AssimpToDXVector(node->mScalingKeys[nextPosition].mValue);
		DirectX::XMVECTOR delta =  DirectX::XMVectorSubtract(end, start);

		return DirectX::XMVectorAdd(start, DirectX::XMVectorScale(delta, factor));

	}

	void SkeletalModel::OnUpdate(float dt) {

		LoadJointsTransforms(dt, m_Transformations);

	}

	void SkeletalModel::OnRender() {

		for (int i = 0; i < m_JointCount; i++) {
			m_Buffer.c_Joints[i] = DX::XMMatrixTranspose(m_Transformations[i]);
		}

		m_Buffer.c_Model = DX::XMMatrixTranspose(m_Transformation);
		m_Buffer.c_View = DX::XMMatrixTranspose(Renderer::GetView());
		m_Buffer.c_Projection = DX::XMMatrixTranspose(Renderer::GetProjection());
		DX::XMFLOAT3 v = Renderer::GetPosition();
		m_Buffer.c_CameraPosition = DX::XMFLOAT4(v.x, v.y, v.z, 1.0f);
		m_Buffer.c_LightPosition = Renderer::GetLight()->GetPosition();
		m_Buffer.c_LightColour = Renderer::GetLight()->GetColour();

		m_CBO->Bind();
		m_CBO->UpdateData(&m_Buffer);

		m_Buffer2.c_AmbientMaterial = m_Material->GetAmbient();
		m_Buffer2.c_DiffuseMaterial = m_Material->GetDiffuse();
		m_Buffer2.c_SpecularMaterial = m_Material->GetSpecular();
		m_Buffer2.c_Shininess = m_Material->GetShininess();
		m_CBO2->BindPS(1);
		m_CBO2->UpdateData(&m_Buffer2);

		m_Material->BindDiffuseTexture(0);
		m_Material->BindNormalTexture(1);

		Renderer::DrawIndexes(&m_Meshes[0]);

	}


	SkeletalModelBuilder::SkeletalModelBuilder() {

	}

	SkeletalModelBuilder SkeletalModelBuilder::GenerateParams() {

		return SkeletalModelBuilder();

	}

	SkeletalModelBuilder& SkeletalModelBuilder::ModelPath(const std::string& path) {

		m_ModelPath = path;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::DiffuseTexture(const std::string& path) {

		m_DiffuseTexture = path;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::SpecularTexture(const std::string& path) {

		m_SpecularTexture = path;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::NormalTexture(const std::string& path) {

		m_NormalTexture = path;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::Position(const DX::XMFLOAT3& position) {

		m_Position = position;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::Rotation(const DX::XMFLOAT3& rotation) {

		m_Rotation = rotation;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::Scale(const DX::XMFLOAT3& scale) {

		m_Scale = scale;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::AmbientMaterial(const DX::XMFLOAT4& ambient) {

		m_AmbientMaterial = ambient;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::DiffuseMaterial(const DX::XMFLOAT4& diffuse) {

		m_DiffuseMaterial = diffuse;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::SpecularMaterial(const DX::XMFLOAT4& specular) {

		m_SpecularMaterial = specular;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::SpecularPowerMaterial(float specularPower) {

		m_SpecularPowerMaterial = specularPower;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::DefaultCollider(bool defaultCollider) {

		m_DefaultCollider = defaultCollider;
		return *this;
	
	}

	SkeletalModelBuilder& SkeletalModelBuilder::AddCollider(const BoxCollider& collider) {

		m_BoxCollider.push_back(collider);
		collidersCount++;
		return *this;

	}

	SkeletalModelBuilder& SkeletalModelBuilder::AddCollider(const std::string& path) {

		m_BoxCollider.push_back({});
		m_BoxCollider[collidersCount].SetAttribs(path);
		collidersCount++;
		return *this;

	}

	std::shared_ptr<SkeletalModel> SkeletalModelBuilder::Build() {

		return std::make_shared<SkeletalModel>(m_ModelPath, m_DiffuseTexture, m_SpecularTexture, m_NormalTexture, m_Position,
			m_Rotation, m_Scale, m_AmbientMaterial, m_DiffuseMaterial, m_SpecularMaterial, m_SpecularPowerMaterial); 

	}

}