#ifndef AGATA_D11_WINDOW_MOVE_EVENT_H
#define AGATA_D11_WINDOW_MOVE_EVENT_H
#pragma once

#include <string>

namespace Agata {

	class WindowMoveEvent {
	public:
		WindowMoveEvent();
		WindowMoveEvent(uint32_t x, uint32_t y);
		virtual ~WindowMoveEvent() = default;

		uint32_t GetX() const;
		uint32_t GetY() const;
		std::string ToString() const;
	private:
		uint32_t m_X, m_Y;
	};

}

#endif