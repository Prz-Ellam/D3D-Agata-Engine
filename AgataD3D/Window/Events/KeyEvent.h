#ifndef AGATA_D11_KEY_EVENT_H
#define AGATA_D11_KEY_EVENT_H
#pragma once

#include "KeyCode.h"
#include <sstream>

namespace Agata {

	class KeyEvent {
	public:
		KeyEvent();
		KeyEvent(KeyCode keyCode, KeyAction state);
		virtual ~KeyEvent() = default;

		KeyCode GetKeyCode() const;
		KeyAction GetKeyAction() const;

		std::string ToString();

		std::string KeyActionToString(KeyAction keyAction);
	private:
		KeyCode m_KeyCode;
		KeyAction m_KeyAction;
	};

}

#endif
