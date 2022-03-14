#ifndef AGATA_D11_MOUSE_BUTTON_EVENT_H
#define AGATA_D11_MOUSE_BUTTON_EVENT_H
#pragma once

#include "KeyCode.h"

namespace Agata {

	class MouseButtonEvent {
	public:
		MouseButtonEvent() = default;
		virtual ~MouseButtonEvent() = default;

	private:
		KeyCode m_KeyCode;
		KeyAction m_KeyAction;
	};

}


#endif
