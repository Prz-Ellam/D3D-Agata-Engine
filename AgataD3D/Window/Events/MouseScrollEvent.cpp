#include "pch.h"
#include "MouseScrollEvent.h"
#include <sstream>

namespace Agata {

	MouseScrollEvent::MouseScrollEvent() : m_X(0), m_Y(0), m_Delta(0) {

	}

	MouseScrollEvent::MouseScrollEvent(uint32_t x, uint32_t y, int delta) : m_X(x), m_Y(y), m_Delta(delta) {

	}

	uint32_t MouseScrollEvent::GetX() const {

		return m_X;

	}

	uint32_t MouseScrollEvent::GetY() const {

		return m_Y;

	}

	std::string MouseScrollEvent::ToString() const {

		std::stringstream ss;
		ss << "Mouse Scroll Event\tX: " << m_X << "\tY: " << m_Y << "\tDelta: " << m_Delta;
		return ss.str();

	}


}