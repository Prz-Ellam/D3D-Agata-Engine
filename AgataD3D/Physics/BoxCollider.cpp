#include "pch.h"
#include "BoxCollider.h"
#include <fstream>

namespace Agata {

	BoxCollider::BoxCollider() : m_Min(0.0f, 0.0f, 0.0f), m_Max(0.0f, 0.0f, 0.0f) {

	}

	BoxCollider::BoxCollider(const DX::XMFLOAT3& min, const DX::XMFLOAT3& max) :
	m_Min(min), m_Max(max) {

	}

	BoxCollider::BoxCollider(const std::string& filePath) {

		std::ifstream reader(filePath);
		reader >> m_Min.x;
		reader >> m_Min.y;
		reader >> m_Min.z;

		reader >> m_Max.x;
		reader >> m_Max.y;
		reader >> m_Max.z;

	}

	void BoxCollider::SetAttribs(const DX::XMFLOAT3& min, const DX::XMFLOAT3& max) {

		m_Min = min;
		m_Max = max;

	}

	void BoxCollider::SetAttribs(const std::string& filePath) {

		std::ifstream reader(filePath);
		reader >> m_Min.x;
		reader >> m_Min.y;
		reader >> m_Min.z;

		reader >> m_Max.x;
		reader >> m_Max.y;
		reader >> m_Max.z;

	}

	void BoxCollider::SetTransformation(const DX::XMMATRIX& transformation) {

		//m_Min = transformation * DX::XMFLOAT4(m_Min, 1.0f);
		//m_Max = transformation * DX::XMFLOAT4(m_Max, 1.0f);

	}

	void BoxCollider::SetMinX(float minX) {

		m_Min.x = minX;
	
	}

	void BoxCollider::SetMaxX(float maxX) {

		m_Max.x = maxX;
	
	}

	void BoxCollider::SetMinY(float minY) {

		m_Min.y = minY;
	
	}

	void BoxCollider::SetMaxY(float maxY) {

		m_Max.y = maxY;
	
	}

	void BoxCollider::SetMinZ(float minZ) {

		m_Min.z = minZ;
	
	}

	void BoxCollider::SetMaxZ(float maxZ) {

		m_Max.z = maxZ;
	
	}

	float BoxCollider::GetMinX() const {

		return m_Min.x;
	
	}

	float BoxCollider::GetMaxX() const {

		return m_Max.x;
	
	}

	float BoxCollider::GetMinY() const {

		return m_Min.y;
	
	}

	float BoxCollider::GetMaxY() const {

		return m_Max.y;
	
	}

	float BoxCollider::GetMinZ() const {

		return m_Min.z;
	
	}

	float BoxCollider::GetMaxZ() const {

		return m_Max.z;
	
	}

	bool BoxCollider::IsColliding(const DX::XMFLOAT3& point) {

		return (point.x >= m_Min.x && point.x <= m_Max.x &&
			point.z >= m_Min.z && point.z <= m_Max.z);

	}

	bool BoxCollider::IsColliding(const BoxCollider& box) {

		return (box.m_Min.x >= m_Max.x && box.m_Max.x <= m_Min.x &&
			box.m_Min.y >= m_Max.y && box.m_Max.y <= m_Min.y &&
			box.m_Min.z >= m_Max.z && box.m_Max.z <= m_Min.z);

	}


}