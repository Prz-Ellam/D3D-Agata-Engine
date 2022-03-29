#ifndef AGATA_D11_DRAWABLE_H
#define AGATA_D11_DRAWABLE_H
#pragma once

#include "Camera.h"
namespace DX = DirectX;

namespace Agata {

	class Drawable {
	public:
		Drawable(const DX::XMFLOAT3& position, const DX::XMFLOAT3& rotation, const DX::XMFLOAT3& scale);
		Drawable(const Drawable& other) = default;
		Drawable(Drawable&& other) noexcept = default;
		virtual ~Drawable() = default;

		Drawable& operator=(const Drawable& other) = default;
		Drawable& operator=(Drawable&& other) noexcept = default;

		virtual void OnRender() = 0;

		DX::XMFLOAT3 GetPosition() const;
		DX::XMFLOAT3 GetRotation() const;
		DX::XMFLOAT3 GetScale() const;

		void SetPosition(const DX::XMFLOAT3& position);
		void SetRotation(const DX::XMFLOAT3& rotation);
		void SetScale(const DX::XMFLOAT3& scale);

		void FollowCamera(std::unique_ptr<Camera>& camera);
	protected:
		DX::XMMATRIX SetMatrix();
		DX::XMFLOAT3 m_Position;
		DX::XMFLOAT3 m_Rotation;
		DX::XMFLOAT3 m_Scale;
		DX::XMMATRIX m_Transformation;
	};

}

#endif
