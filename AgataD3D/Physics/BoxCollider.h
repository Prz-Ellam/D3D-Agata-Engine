#ifndef AGATA_BOX_COLLIDER_H
#define AGATA_BOX_COLLIDER_H
#pragma once

#include <DirectXMath.h>
#include <string>

namespace DX = DirectX;

namespace Agata {

	class BoxCollider {
	public:
		BoxCollider();
		BoxCollider(const DX::XMFLOAT3& min, const DX::XMFLOAT3& max);
		BoxCollider(const std::string& filePath);

		void SetAttribs(const DX::XMFLOAT3& min, const DX::XMFLOAT3& max);
		void SetAttribs(const std::string& filePath);
		void SetTransformation(const DX::XMMATRIX& transformation);

		void SetMinX(float minX);
		void SetMaxX(float maxX);
		void SetMinY(float minY);
		void SetMaxY(float maxY);
		void SetMinZ(float minZ);
		void SetMaxZ(float maxZ);

		float GetMinX() const;
		float GetMaxX() const;
		float GetMinY() const;
		float GetMaxY() const;
		float GetMinZ() const;
		float GetMaxZ() const;

		bool IsColliding(const DX::XMFLOAT3& point);
		bool IsColliding(const BoxCollider& box);
	private:
		DX::XMFLOAT3 m_Min;
		DX::XMFLOAT3 m_Max;
	};

}

#endif
