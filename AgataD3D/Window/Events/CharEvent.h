#ifndef AGATA_D11_CHAR_EVENT_H
#define AGATA_D11_CHAR_EVENT_H
#pragma once

#include "KeyCode.h"
#include <sstream>

namespace Agata {

	class CharEvent {
	public:
		CharEvent();
		CharEvent(KeyCode keyCode);
		virtual ~CharEvent() = default;

		KeyCode GetKeyCode() const;

		std::string ToString();

	private:
		KeyCode m_KeyCode;
	};

}

#endif
