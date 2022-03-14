#include "pch.h"
#include "WindowResizeEvent.h"

namespace Agata {

	WindowResizeEvent::WindowResizeEvent() : m_Width(0), m_Height(0) {

	}

	WindowResizeEvent::WindowResizeEvent(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height) {

	}

	uint32_t WindowResizeEvent::GetWidth() const {

		return m_Width;

	}

	uint32_t WindowResizeEvent::GetHeight() const {

		return m_Height;

	}

	std::string WindowResizeEvent::ToString() const {

		std::stringstream ss;
		ss << "Window Resize Event\tWidth: " << m_Width << "\tHeight: " << m_Height;
		return ss.str();

	}

}