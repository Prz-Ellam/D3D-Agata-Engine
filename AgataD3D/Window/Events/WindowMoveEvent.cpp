#include "pch.h"
#include "WindowMoveEvent.h"
#include <sstream>

namespace Agata {

	WindowMoveEvent::WindowMoveEvent() : m_X(0), m_Y(0) {

	}

	WindowMoveEvent::WindowMoveEvent(uint32_t x, uint32_t y) : m_X(x), m_Y(y) {

	}

	uint32_t WindowMoveEvent::GetX() const {

		return m_X;

	}

	uint32_t WindowMoveEvent::GetY() const {

		return m_Y;

	}

	std::string WindowMoveEvent::ToString() const {

		std::stringstream ss;
		ss << "Window Move Event\tX: " << m_X << "\tY:" << m_Y;
		return ss.str();

	}

}