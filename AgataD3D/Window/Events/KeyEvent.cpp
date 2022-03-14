#include "pch.h"
#include "KeyEvent.h"

namespace Agata {

	KeyEvent::KeyEvent() : m_KeyCode(KeyCode::KeyEnter), m_KeyAction(KeyAction::Release) {

	}

	KeyEvent::KeyEvent(KeyCode keyCode, KeyAction state) : m_KeyCode(keyCode), m_KeyAction(state) {

	}

	std::string KeyEvent::ToString() {

		std::stringstream ss;
		ss << "Key Event\tKeyCode: " << std::to_string(static_cast<int>(m_KeyCode)) <<
			"\tKeyAction: " << KeyActionToString(m_KeyAction);

		return ss.str();

	}

	std::string KeyEvent::KeyActionToString(KeyAction keyAction) {

		switch (keyAction) {
			case KeyAction::Release:		return "Release";
			case KeyAction::Press:			return "Press";
			case KeyAction::Repeat:			return "Repeat";
			default:						return "Unknown";
		}

		return "Unknown";

	}

	KeyCode KeyEvent::GetKeyCode() const {

		return m_KeyCode;

	}

	KeyAction KeyEvent::GetKeyAction() const {

		return m_KeyAction;

	}


}