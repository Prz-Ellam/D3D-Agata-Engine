#include "pch.h"
#include "CharEvent.h"

namespace Agata {

	CharEvent::CharEvent() : m_KeyCode(KeyCode::KeyEnter) {

	}

	CharEvent::CharEvent(KeyCode keyCode) : m_KeyCode(keyCode) {

	}

	std::string CharEvent::ToString() {

		std::stringstream ss;
		ss << "Key Event\tKeyCode: " << std::to_string(static_cast<int>(m_KeyCode));

		return ss.str();

	}

	KeyCode CharEvent::GetKeyCode() const {

		return m_KeyCode;

	}

}