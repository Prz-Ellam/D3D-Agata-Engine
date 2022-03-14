/*
    Joystick.h
	Proposito: Manejar la obtencion de inputs de los controles
	@autor Eliam Perez
	Agata Engine Direct3D 11
*/

#ifndef AGATA_D11_JOYSTICK_H
#define AGATA_D11_JOYSTICK_H
#pragma once

#include <XInput.h>

namespace Agata {

	/* 
	   Estructura que contiene todos los Inputs del Control, para evitar las mascaras de
       bits y crear una API mas amigable se utiliza un Campo de Bits
	*/
	struct JoystickState {
		bool DPadUp : 1;
		bool DPadDown : 1;
		bool DPadLeft : 1;
		bool DPadRight : 1;
		bool Start : 1;
		bool Back : 1;
		bool LeftThumb : 1;
		bool RightThumb : 1;
		bool LeftShoulder : 1;
		bool RightShoulder : 1;
		bool : 2;
		bool A : 1;
		bool B : 1;
		bool X : 1;
		bool Y : 1;
		float leftTrigger;
		float rightTrigger;
		float thumbLX;
		float thumbLY;
		float thumbRX;
		float thumbRY;
	};

	class Joystick {
	public:
		Joystick() = default;

		// Obtiene el estado de los inputs de un determado control
		static JoystickState GetState(int index = 0);

		// Verifica si un control se encuentra conectado
		static bool Present(int index = 0);
	};

}

#endif // !AGATA_D11_JOYSTICK_H
