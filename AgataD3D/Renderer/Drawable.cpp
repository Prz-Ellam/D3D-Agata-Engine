#include "pch.h"
#include "Drawable.h"

namespace Agata {

	Drawable::Drawable(const DX::XMFLOAT3& position, const DX::XMFLOAT3& rotation, 
		const DX::XMFLOAT3& scale) : m_Position(position), m_Rotation(rotation), m_Scale(scale), 
		m_Transformation(SetMatrix()) {}

	DX::XMFLOAT3 Drawable::GetPosition() const {

		return m_Position;

	}

	DX::XMFLOAT3 Drawable::GetRotation() const {

		return m_Rotation;

	}

	DX::XMFLOAT3 Drawable::GetScale() const {

		return m_Scale;

	}

	void Drawable::SetPosition(const DX::XMFLOAT3& position) {

		m_Position = position;

	}

	void Drawable::SetRotation(const DX::XMFLOAT3& rotation) {

		m_Rotation = rotation;

	}

	void Drawable::SetScale(const DX::XMFLOAT3& scale) {

		m_Scale = scale;

	}

	DX::XMMATRIX Drawable::SetMatrix() {

		return DX::XMMatrixRotationX(m_Rotation.x) *
			DX::XMMatrixRotationX(m_Rotation.y) *
			DX::XMMatrixRotationX(m_Rotation.z) * 
			DX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
			DX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	}

}