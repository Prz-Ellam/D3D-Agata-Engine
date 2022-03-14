#include "pch.h"
#include "Joystick.h"

namespace Agata {

	/*
	    Se obtiene el estado del control con XINPUT_STATE y se mapea a la estructura 
		propia de JoystickState, los valores de los triggers y los thumbs se normaliza
	*/
	JoystickState Joystick::GetState(int index) {

		XINPUT_STATE m_InputState;
		if (XInputGetState(index, &m_InputState) == ERROR_DEVICE_NOT_CONNECTED) {
			return {};
		}

		JoystickState state;
		memcpy(&state, &m_InputState.Gamepad.wButtons, sizeof(WORD));
		state.leftTrigger = static_cast<float>(m_InputState.Gamepad.bLeftTrigger) / UCHAR_MAX;
		state.rightTrigger = static_cast<float>(m_InputState.Gamepad.bRightTrigger) / UCHAR_MAX;
		state.thumbLX = static_cast<float>(m_InputState.Gamepad.sThumbLX) / INT16_MAX;
		state.thumbLY = static_cast<float>(m_InputState.Gamepad.sThumbLY) / INT16_MAX;
		state.thumbRX = static_cast<float>(m_InputState.Gamepad.sThumbRX) / INT16_MAX;
		state.thumbRY = static_cast<float>(m_InputState.Gamepad.sThumbRY) / INT16_MAX;

		return state;

	}

	/*
	    Se manda a llamar XInputGetState, si devuelve ERROR_SUCCESS significa que el
		control esta en funcionamiento, caso opuesto esta desoperativo
	*/
	bool Joystick::Present(int index) {

		XINPUT_STATE m_InputState = {};
		if (XInputGetState(index, &m_InputState) == ERROR_SUCCESS) {
			return true;
		}
		else {
			return false;
		}

	}


}