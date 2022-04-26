#include "pch.h"
#include "MathUtils.h"

namespace Agata {

	float MathUtils::BaryCentricCoordinatesY(const DX::XMFLOAT3& p1, const DX::XMFLOAT3& p2,
		const DX::XMFLOAT3& p3, const DX::XMFLOAT2& pos) {

		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;

	}

	DirectX::XMMATRIX MathUtils::AssimpToDXMatrix(aiMatrix4x4 m) {

		return DirectX::XMMATRIX(
			m.a1, m.b1, m.c1, m.d1,
			m.a2, m.b2, m.c2, m.d2,
			m.a3, m.b3, m.c3, m.d3,
			m.a4, m.b4, m.c4, m.d4
		);

	}

	DirectX::XMVECTOR MathUtils::AssimpToDXVector(const aiVector3D& v) {

		DirectX::XMFLOAT3 vector = DirectX::XMFLOAT3(v.x, v.y, v.z);
		return DirectX::XMLoadFloat3(&vector);

	}

	DirectX::XMVECTOR MathUtils::AssimpToDXQuat(const aiQuaternion& quat) {

		DirectX::XMFLOAT4 quaternion = DirectX::XMFLOAT4(quat.x, quat.y, quat.z, quat.w);
		return DirectX::XMLoadFloat4(&quaternion);

	}

	DirectX::XMMATRIX MathUtils::TransformationMatrix(DirectX::XMVECTOR _position,
		DirectX::XMVECTOR _rotation, DirectX::XMVECTOR _scale) {

		DirectX::XMFLOAT3 pos, scale;
		DirectX::XMFLOAT4 quat;

		DirectX::XMStoreFloat3(&pos, _position);
		DirectX::XMStoreFloat4(&quat, _rotation);
		DirectX::XMStoreFloat3(&scale, _scale);

		return DirectX::XMMATRIX(
			scale.x * (1 - 2 * quat.y * quat.y - 2 * quat.z * quat.z), 
			scale.x * (2 * quat.y * quat.x + 2 * quat.z * quat.w), 
			scale.x * (2 * quat.z * quat.x - 2 * quat.y * quat.w), 
			0,
			scale.y * (2 * quat.y * quat.x - 2 * quat.z * quat.w), 
			scale.y * (1 - 2 * quat.z * quat.z - 2 * quat.x * quat.x), 
			scale.y * (2 * quat.y * quat.z + 2 * quat.x * quat.w), 
			0,
			scale.z * (2 * quat.y * quat.w + 2 * quat.z * quat.x), 
			scale.z * (2 * quat.y * quat.z - 2 * quat.x * quat.w), 
			scale.z * (1 - 2 * quat.y * quat.y - 2 * quat.x * quat.x), 
			0,
			pos.x, 
			pos.y, 
			pos.z, 
			1
		);

	}

}