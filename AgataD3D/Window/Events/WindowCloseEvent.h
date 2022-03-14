#ifndef AGATA_D11_WINDOW_CLOSE_EVENT_H
#define AGATA_D11_WINDOW_CLOSE_EVENT_H
#pragma once

#include <string>

namespace Agata {

	class WindowCloseEvent {
	public:
		WindowCloseEvent() = default;
		virtual ~WindowCloseEvent() = default;
		std::string ToString() const;
	};

}

#endif
