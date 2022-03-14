/*
    Window.h
	Proposito: Manejar las ventanas de la aplicacion
	@autor Eliam Perez
	Agata Engine Direct3D 11
*/

#ifndef AGATA_D11_WINDOW_H
#define AGATA_D11_WINDOW_H
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include <mmsystem.h>

#include "Events/WindowEvents.h"

namespace Agata {

	using WindowResizeEventFn = std::function<void(WindowResizeEvent)>;
	using WindowMoveEventFn = std::function<void(WindowMoveEvent)>;
	using WindowCloseEventFn = std::function<void(WindowCloseEvent)>;
	using MouseMoveEventFn = std::function<void(MouseMoveEvent)>;
	using MouseScrollEventFn = std::function<void(MouseScrollEvent)>;
	using MouseButtonEventFn = std::function<void(MouseButtonEvent)>;
	using KeyEventFn = std::function<void(KeyEvent)>;
	using CharEventFn = std::function<void(CharEvent)>;
	using JoyStickEventFn = std::function<void(JoyStickEvent)>;

	struct WindowParams {
		std::string Title = "Agata Engine";
		uint32_t Width = 1280u;
		uint32_t Height = 720u;
	};

	class Window {
	public:
		friend class Scene;
		Window();
		Window(const WindowParams& params);
		Window(uint32_t width, uint32_t height, const std::string& title);
		Window(const Window& other) = default;
		Window(Window&& other) noexcept = default;
		virtual ~Window();

		Window& operator=(const Window& other) = default;
		Window& operator=(Window&& other) noexcept = default;

		void Show();
		void Hide();
		void Update();

		HWND GetHandler() const;

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		std::string GetTitle() const;
		void SetTitle(const std::string& title);
		void SetIcon(const std::string& path);

		void ShowCursor();
		void HideCursor();

		bool GetVSync() const;
		void SetVSync(bool vSync);
	private:
		bool RegisterWindowClass();
		bool InitWindow();
		void ReleaseWindow();
		void SetDefaultHandlers();
		void InitJoystick();

		static LRESULT CALLBACK StaticMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		HWND m_Handler;
		uint32_t m_Width, m_Height;
		std::string m_Title;
		bool m_VSync;
		bool m_CursorVisible;

		// Eventos manejados por las ventanas
		WindowResizeEventFn m_WindowResizeEventHandler;
		WindowMoveEventFn m_WindowMoveEventHandler;
		MouseMoveEventFn m_MouseMoveEventHandler;
		MouseScrollEventFn m_MouseScrollEventHandler;
		WindowCloseEventFn m_WindowCloseEventHandler;
		KeyEventFn m_KeyEventHandler;
		MouseButtonEventFn m_MouseButtonEventHandler;
		CharEventFn m_CharEventHandler;
		JoyStickEventFn m_JoyStickEventHandler;

		void SetWindowResizeEventHandler(WindowResizeEventFn handler);
		void SetWindowMoveEventHandler(WindowMoveEventFn handler);
		void SetMouseMoveEventHandler(MouseMoveEventFn handler);
		void SetMouseScrollEventHandler(MouseScrollEventFn handler);
		void SetWindowCloseEventHandler(WindowCloseEventFn handler);
		void SetKeyEventHandler(KeyEventFn handler);
		void SetMouseButtonEventHandler(MouseButtonEventFn handler);
		void SetCharEventHandler(CharEventFn handler);
		void SetJoyStickEventHandler(JoyStickEventFn handler);

		std::map<uint32_t, KeyCode> m_MouseButtons;

		static HINSTANCE s_Instance;
		static const std::string s_WindowClassName;
		static bool s_WindowClassRegistered;
		static uint32_t s_WindowsCount;
	};

}

#endif // !AGATA_D11_WINDOW_H