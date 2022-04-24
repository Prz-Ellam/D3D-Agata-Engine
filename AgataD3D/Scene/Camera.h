#ifndef AGATA_D11_CAMERA_H
#define AGATA_D11_CAMERA_H
#pragma once

#include "Window.h"

#ifdef AGATA_D11
#define HAND -1
#else
#define HAND 1
#endif

namespace Agata {

	struct PerspectiveCameraProps {
		float Fov;
		float Aspect;
		float NearPlane, FarPlane;
	};

	class Terrain;

	class Camera {
	private:
		PerspectiveCameraProps m_Properties;
		DirectX::XMVECTOR m_Position;
		DirectX::XMVECTOR m_PrevPosition;
		DirectX::XMVECTOR m_ThirdPosition;
		DirectX::XMVECTOR m_Forward, m_Right, m_Up;
		float m_Pitch, m_Yaw;
		float m_Speed, m_Sensitivity;
		DirectX::XMMATRIX m_View, m_Projection;

		float m_YawVelocity, m_YawAcceleration;
		float m_PitchVelocity, m_PitchAcceleration;

		float m_ForwardVelocity, m_ForwardAcceleration;
		float m_RightVelocity, m_RightAcceleration;

		float m_UpVelocity, m_UpAcceleration;

		float m_LastX, m_LastY;
		bool m_FirstMouse;
		bool m_FirstPerson;

	public:
		Camera(const PerspectiveCameraProps& properties, float speed, float sensitivity);
		virtual ~Camera() = default;

		// Actualiza los angulos yaw, pitch, roll y la posicion relativa al mundo
		void Move(float dt); // Inputs de Teclado, WASD y Flechas por default
		void Move(int x, int y); // Inputs de Mouse

		// Actualiza las matrices de proyeccion y vista
		void Update(std::shared_ptr<Terrain>& terrain);

		void SetSpeed(float speed);
		void SetSensitivity(float sensitivity);

		void MoveHeight(float height);
		void ChangePitchDirection();

		void Back();

		DirectX::XMMATRIX GetView() const;
		DirectX::XMMATRIX GetProjection() const;

		float GetYaw() const;

		void TogglePerson();
		void SetFirstPerson();
		void SetThirdPerson();

		DirectX::XMFLOAT3 GetPosition() const;
		DirectX::XMFLOAT3 GetDirection() const;
		float GetX() const;
		float GetZ() const;
		float GetY() const;
		void SetY(float y);

		PerspectiveCameraProps& GetPerspectiveCameraPropsRef();
	};

}

#endif
