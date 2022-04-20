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

}