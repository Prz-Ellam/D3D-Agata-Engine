#ifndef AGATA_D11_WINDOW_RESIZE_EVENT_H
#define AGATA_D11_WINDOW_RESIZE_EVENT_H
#pragma once

#include <stdint.h>
#include <string>

namespace Agata {

	class WindowResizeEvent {
	public:
		WindowResizeEvent();
		WindowResizeEvent(uint32_t width, uint32_t height);
		virtual ~WindowResizeEvent() = default;

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		std::string ToString() const;
	private:
		uint32_t m_Width, m_Height;
	};

}

#endif
