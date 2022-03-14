#ifndef AGATA_D11_MOUSE_SCROLL_EVENT_H
#define AGATA_D11_MOUSE_SCROLL_EVENT_H
#pragma once

#include <stdint.h>
#include <string>

namespace Agata {

	class MouseScrollEvent {
	public:
		MouseScrollEvent();
		MouseScrollEvent(uint32_t x, uint32_t y, int delta);
		virtual ~MouseScrollEvent() = default;

		uint32_t GetX() const;
		uint32_t GetY() const;
		std::string ToString() const;
	private:
		uint32_t m_X, m_Y;
		int m_Delta;
	};

}

#endif
