#ifndef AGATA_D11_DIRECTION_LIGHT_H
#define AGATA_D11_DIRECTION_LIGHT_H
#pragma once

namespace Agata {

	class DirectionLight {
	public:
		DirectionLight(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& colour);
		DirectionLight(const DirectionLight& other) = default;
		DirectionLight(DirectionLight&& other) noexcept = default;
		virtual ~DirectionLight() = default;

		DirectionLight& operator=(const DirectionLight& other) = default;
		DirectionLight& operator=(DirectionLight&& other) = default;

		DirectX::XMFLOAT4 GetPosition() const;
		DirectX::XMFLOAT4 GetColour() const;

		void SetPositionX(float x);
		void SetPositionY(float y);
		void SetPositionZ(float z);

		void SetColourR(float r);
		void SetColourG(float g);
		void SetColourB(float b);
	private:
		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT3 m_Colour;
	};

}

#endif
