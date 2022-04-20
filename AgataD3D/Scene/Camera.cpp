#include "pch.h"
#include "Camera.h"
#include "Terrain.h"
#include "Joystick.h"
#include "Events/KeyCode.h"

namespace Agata {

	Camera::Camera(const PerspectiveCameraProps& properties, float speed, float sensitivity)
		: m_Position(DX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0F)), m_Speed(speed), 
		m_Sensitivity(sensitivity), m_Properties(properties){

		m_Forward = DX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		m_Up = DX::XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
		m_Right = DX::XMVector3Normalize(DirectX::XMVector3Cross(m_Forward, m_Up));
		
		m_View = DX::XMMatrixLookAtLH(m_Position, DX::XMVectorAdd(m_Position, m_Forward), m_Up);
		m_Projection = DX::XMMatrixPerspectiveFovLH(DX::XMConvertToRadians(m_Properties.Fov),
			m_Properties.Aspect, m_Properties.NearPlane, m_Properties.FarPlane);

		m_Pitch = 0.0f;
		m_Yaw = 90.0f;

		m_YawVelocity = 0.0f;
		m_YawAcceleration = 0.0f;
		m_PitchVelocity = 0.0f;
		m_PitchAcceleration = 0.0f;
		m_ForwardVelocity = 0.0f;
		m_ForwardAcceleration = 0.0f;
		m_RightVelocity = 0.0f;
		m_RightAcceleration = 0.0f;
		m_UpVelocity = 0.0f;
		m_FirstPerson = false;

	}

	void Camera::Move(float dt) {

		m_PrevPosition = m_Position;

		m_ForwardAcceleration = 0.0f;
		m_RightAcceleration = 0.0f;
		if (GetAsyncKeyState(static_cast<int>('W')) & 1 << 16) {
			//Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Pasos")->second, 1, 0.30f);
			m_ForwardAcceleration += m_Speed;
		}
		if (GetAsyncKeyState(static_cast<int>('S')) & 1 << 16) {
			//Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Pasos")->second, 1, 0.30f);
			m_ForwardAcceleration -= m_Speed;
		}
		if (m_FirstPerson) {
			if (GetAsyncKeyState(static_cast<int>('A')) & 1 << 16) {
				//Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Pasos")->second, 1, 0.30f);
				m_RightAcceleration -= m_Speed * HAND;
			}
			if (GetAsyncKeyState(static_cast<int>('D')) & 1 << 16) {
				//Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Pasos")->second, 1, 0.30f);
				m_RightAcceleration += m_Speed * HAND;
			}
		}

		m_YawAcceleration = 0.0f;
		m_PitchAcceleration = 0.0f;
		if (GetAsyncKeyState(static_cast<int>(KeyCode::KeyLeft)) & 1 << 16) {
			m_YawAcceleration -= m_Sensitivity * HAND;
		}
		if (GetAsyncKeyState(static_cast<int>(KeyCode::KeyRight)) & 1 << 16) {
			m_YawAcceleration += m_Sensitivity * HAND;
		}
		if (GetAsyncKeyState(static_cast<int>(KeyCode::KeyUp)) & 1 << 16) {
			m_PitchAcceleration -= m_Sensitivity * HAND;
		}
		if (GetAsyncKeyState(static_cast<int>(KeyCode::KeyDown)) & 1 << 16) {
			m_PitchAcceleration += m_Sensitivity * HAND;
		}

		

		if (Joystick::Present(0)) {
			
			JoystickState state = Joystick::GetState(0);

			if (state.thumbLX > 0.19 || state.thumbLX < -0.19) {
				//Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Pasos")->second, 1, 0.30f);
				m_RightAcceleration += state.thumbLX * m_Speed * HAND;
			}
			if (state.thumbLY > 0.19 || state.thumbLY < -0.19) {
				//Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Pasos")->second, 1, 0.30f);
				m_ForwardAcceleration -= state.thumbLY * m_Speed * HAND;
			}
			
			if (state.thumbRX > 0.19 || state.thumbRX < -0.19) {
				m_YawAcceleration += state.thumbRX * m_Sensitivity * HAND;
			}
			if (state.thumbRY > 0.19 || state.thumbRY < -0.19) {
				m_PitchAcceleration -= state.thumbRY * m_Sensitivity * HAND;
			}

		}


		m_ForwardAcceleration -= m_ForwardVelocity * 10.0f;
		m_Position = DX::XMVectorAdd(m_Position, DX::XMVectorScale(m_Forward, 
			m_ForwardVelocity * dt + m_ForwardAcceleration * dt * dt * 0.5f));
		m_ForwardVelocity = m_ForwardVelocity + m_ForwardAcceleration * dt;

		m_RightAcceleration -= m_RightVelocity * 10.0f;

		m_Position = DX::XMVectorAdd(m_Position, DX::XMVectorScale(m_Right, 
			m_RightVelocity * dt + m_RightVelocity * dt * dt * 0.5f));

		m_RightVelocity = m_RightVelocity + m_RightAcceleration * dt;

		m_PitchAcceleration -= m_PitchVelocity * 12.0f;
		m_Pitch = std::clamp(m_Pitch + m_PitchVelocity * dt + m_PitchAcceleration * dt * dt * 0.5f, -89.0f, 89.0f);
		m_PitchVelocity = m_PitchVelocity + m_PitchAcceleration * dt;

		m_YawAcceleration -= m_YawVelocity * 12.0f;
		m_Yaw = m_Yaw + m_YawVelocity * dt + m_YawAcceleration * dt * dt * 0.5f;
		m_YawVelocity = m_YawVelocity + m_YawAcceleration * dt;

		//if (m_Position.x > 200.0f || m_Position.x < -200.0f || m_Position.z > 200.0f || m_Position.z < -200.0f) {
		//	m_Position = m_PrevPosition;
		//}

	}

	void Camera::Move(int x, int y) {

		if (m_FirstMouse) {
			m_LastX = x;
			m_LastY = y;
			m_FirstMouse = false;
		}

		float xoffset = x - m_LastX;
		float yoffset = m_LastY - y;
		m_LastX = x;
		m_LastY = y;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_Yaw += xoffset;
		
		m_Pitch += std::clamp(yoffset, -89.0f, 89.0f);

		DX::XMVECTOR direction = DX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		direction = DX::XMVectorSetX(direction, DX::XMScalarCos(DX::XMConvertToRadians(m_Yaw)) * 
			DX::XMScalarCos(DX::XMConvertToRadians(m_Pitch)));
		direction = DX::XMVectorSetY(direction, DX::XMScalarSin(DX::XMConvertToRadians(m_Pitch)));
		direction = DX::XMVectorSetZ(direction, DX::XMScalarSin(DX::XMConvertToRadians(m_Yaw)) * 
			DX::XMScalarCos(DX::XMConvertToRadians(m_Pitch)));

		m_Forward =  DX::XMVector3Normalize(direction);
		m_Right = DX::XMVector3Normalize(DX::XMVector3Cross(m_Forward, m_Up));

		m_View = DX::XMMatrixLookAtLH(m_Position, DX::XMVectorAdd(m_Position, m_Forward), m_Up);
		m_Projection = DX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_Properties.Fov),
			m_Properties.Aspect, m_Properties.NearPlane, m_Properties.FarPlane);


	}

	void Camera::Update() {

		DX::XMVECTOR direction = DX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		direction = DX::XMVectorSetX(direction, DX::XMScalarCos(DX::XMConvertToRadians(m_Yaw)) * 
			DX::XMScalarCos(DX::XMConvertToRadians(m_Pitch)));
		direction = DX::XMVectorSetY(direction, DX::XMScalarSin(DX::XMConvertToRadians(m_Pitch)));
		direction = DX::XMVectorSetZ(direction, DX::XMScalarSin(DX::XMConvertToRadians(m_Yaw)) * 
			DX::XMScalarCos(DX::XMConvertToRadians(m_Pitch)));

		m_Forward =  DX::XMVector3Normalize(direction);
		m_Right = DX::XMVector3Normalize(DX::XMVector3Cross(m_Forward, m_Up));

		// Altura promedio de los ojos de una persona de 1.8m
		//m_Position.y = terrain.getHeight(m_Position.x, m_Position.z) + 1.665f;

		m_Projection = DX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_Properties.Fov),
			m_Properties.Aspect, m_Properties.NearPlane, m_Properties.FarPlane);

		if (m_FirstPerson) {
			m_View = DX::XMMatrixLookAtLH(m_Position, DX::XMVectorAdd(m_Position, m_Forward), m_Up);
		}
		else {
			m_ThirdPosition = DX::XMVectorAdd(m_Position, DX::XMVectorScale(m_Forward, -5.0f));
			m_View = DX::XMMatrixLookAtLH(m_ThirdPosition, DX::XMVectorAdd(m_ThirdPosition, m_Forward), m_Up);
		}

	}

	void Camera::SetSpeed(float speed) {

		m_Speed = speed;

	}

	void Camera::SetSensitivity(float sensitivity) {

		m_Sensitivity = sensitivity;

	}

	void Camera::MoveHeight(float height) {

		DX::XMFLOAT3 v;
		DX::XMStoreFloat3(&v, m_Position);
		v.y += height;
		m_Position = DX::XMLoadFloat3(&v);
		m_View = DX::XMMatrixLookAtLH(m_Position, DX::XMVectorAdd(m_Position, m_Forward), m_Up);

	}

	void Camera::ChangePitchDirection() {

		m_Pitch = -m_Pitch;

	}

	DirectX::XMMATRIX Camera::GetView() const {

		return m_View;

	}

	DirectX::XMMATRIX Camera::GetProjection() const {

		return m_Projection;

	}

	DX::XMFLOAT3 Camera::GetPosition() const {

		DX::XMFLOAT3 v;
		DX::XMStoreFloat3(&v, m_Position);
		return v;

	}

	float Camera::GetX() const {

		return DX::XMVectorGetX(m_Position);

	}

	float Camera::GetZ() const {

		return DX::XMVectorGetZ(m_Position);

	}

	float Camera::GetY() const {

		return DX::XMVectorGetY(m_Position);

	}

	void Camera::SetY(float y) {

		DX::XMFLOAT3 v;
		DX::XMStoreFloat3(&v, m_Position);
		v.y = y + 1.665;
		m_Position = DX::XMLoadFloat3(&v);

	}

	DX::XMFLOAT3 Camera::GetDirection() const {

		DX::XMFLOAT3 v;
		DX::XMStoreFloat3(&v, m_Forward);

		return v;

	}

	float Camera::GetYaw() const {

		return m_Yaw;

	}

	void Camera::TogglePerson() {

		m_FirstPerson = !m_FirstPerson;

	}

	void Camera::SetFirstPerson() {

		m_FirstPerson = true;

	}

	void Camera::SetThirdPerson() {

		m_FirstPerson = false;

	}

}