#include "pch.h"
#include "MouseMoveEvent.h"

namespace Agata {

	MouseMoveEvent::MouseMoveEvent() : m_X(0), m_Y(0) {

	}

	MouseMoveEvent::MouseMoveEvent(uint32_t x, uint32_t y) : m_X(x), m_Y(y) {

	}

	uint32_t MouseMoveEvent::GetX() const {

		return m_X;

	}

	uint32_t MouseMoveEvent::GetY() const {

		return m_Y;

	}

	std::string MouseMoveEvent::ToString() const {

		std::stringstream ss;
		ss << "Mouse Move Event\tX: " << m_X << "\tY:" << m_Y;
		return ss.str();

	}

}