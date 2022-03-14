#ifndef AGATA_D11_KEY_CODE_H
#define AGATA_D11_KEY_CODE_H
#pragma once

namespace Agata {

	enum class KeyCode {

		LeftButton = 1, RightButton = 2, MiddleButton = 4,
		X1Button = 5, X2Button = 6,

		KeyBackSpace = 8, KeyTab = 9, KeyClear = 12, KeyEnter = 13,
		KeyShift = 16, KeyControl = 17, KeyAlt = 18, KeyPause = 19,
		KeyCapital = 20, 
		
		
		KeyEscape = 27, KeySpace = 32, KeyPrior = 33, KeyNext = 34, 
		KeyEnd = 35, KeyHome = 36,

		KeyLeft = 37, KeyUp = 38, KeyRight = 39, KeyDown = 40,

		// 0 - 9
		Key0 = '0', Key1 = '1', Key2 = '2', Key3 = '3', Key4 = '4', Key5 = '5',
		Key6 = '6', Key7 = '7', Key8 = '8', Key9 = '9',

		// A - Z
		KeyA = 'A', KeyB = 'B', KeyC = 'C', KeyD = 'D', KeyE = 'E', KeyF = 'F', KeyG = 'G', KeyH = 'H',
		KeyI = 'I', KeyJ = 'J', KeyK = 'K', KeyL = 'L', KeyM = 'M', KeyN = 'N', KeyO = 'O', KeyP = 'P',
		KeyQ = 'Q', KeyR = 'R', KeyS = 'S', KeyT = 'T', KeyU = 'U', KeyV = 'V', KeyW = 'W', KeyX = 'X',
		KeyY = 'Y', KeyZ = 'Z'

	};

	enum class JoyStickCode {
		ButtonA = 0x0001,
		ButtonB = 0x0002,
		ButtonX = 0x0003,
		ButtonY = 0x0004,
	};

	enum class KeyAction {
		Release = 0,
		Press = 1,
		Repeat = 2
	};

}

#endif