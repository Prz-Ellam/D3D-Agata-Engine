#ifndef AGATA_D11_MATH_UTILS_H
#define AGATA_D11_MATH_UTILS_H
#pragma once

#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <DirectXMath.h>

namespace DX = DirectX;

namespace Agata {

	class MathUtils {
	public:
		MathUtils() = delete;
		MathUtils(const MathUtils& other) = delete;
		MathUtils(MathUtils&& other) noexcept = delete;

		static DirectX::XMMATRIX AssimpToDXMatrix(aiMatrix4x4 matrix);
		static DirectX::XMVECTOR AssimpToDXVector(const aiVector3D& vector);
		static DirectX::XMVECTOR AssimpToDXQuat(const aiQuaternion& quat);

		static DirectX::XMMATRIX TransformationMatrix(DirectX::XMVECTOR position,
			DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale);

		static float BaryCentricCoordinatesY(const DX::XMFLOAT3& p1, const DX::XMFLOAT3& p2, 
			const DX::XMFLOAT3& p3, const DX::XMFLOAT2& pos);
	};

}

#endif
