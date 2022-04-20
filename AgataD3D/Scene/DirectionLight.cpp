#include "pch.h"
#include "DirectionLight.h"

namespace Agata {

	DirectionLight::DirectionLight(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& colour)
	: m_Position(position), m_Colour(colour) {

	}

	DirectX::XMFLOAT4 DirectionLight::GetPosition() const {

		return DirectX::XMFLOAT4(m_Position.x, m_Position.y, m_Position.z, 1.0f);

	}

	DirectX::XMFLOAT4 DirectionLight::GetColour() const {

		return DirectX::XMFLOAT4(m_Colour.x, m_Colour.y, m_Colour.z, 1.0f);

	}

	void DirectionLight::SetPositionX(float x) {

		m_Position.x = x;

	}

	void DirectionLight::SetPositionY(float y) {

		m_Position.y = y;

	}

	void DirectionLight::SetPositionZ(float z) {

		m_Position.z = z;

	}

	void DirectionLight::SetColourR(float r) {

		m_Colour.x = r;

	}

	void DirectionLight::SetColourG(float g) {

		m_Colour.y = g;

	}

	void DirectionLight::SetColourB(float b) {

		m_Colour.z = b;

	}

}