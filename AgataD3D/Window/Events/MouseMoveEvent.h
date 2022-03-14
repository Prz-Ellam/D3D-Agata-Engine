#ifndef AGATA_D11_MOUSE_MOVE_EVENT_H
#define AGATA_D11_MOUSE_MOVE_EVENT_H
#pragma once

#include <stdint.h>
#include <string>

namespace Agata {

	class MouseMoveEvent {
	public:
		MouseMoveEvent();
		MouseMoveEvent(uint32_t x, uint32_t y);
		virtual ~MouseMoveEvent() = default;

		uint32_t GetX() const;
		uint32_t GetY() const;
		std::string ToString() const;
	private:
		uint32_t m_X, m_Y;
	};

}

#endif
