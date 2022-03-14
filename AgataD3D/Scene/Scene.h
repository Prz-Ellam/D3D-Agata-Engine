#ifndef AGATA_D11_SCENE_H
#define AGATA_D11_SCENE_H
#pragma once

#include "Window.h"
#include "Renderer.h"

namespace Agata {

#define EVENT_FN(name) std::bind(&##name, this, std::placeholders::_1)

	class Scene {
	public:
		Scene(const WindowParams& windowParams = WindowParams());
		Scene(const Scene& rhs) = default;
		Scene(Scene&& rhs) noexcept = default;
		virtual ~Scene();

		Scene& operator=(const Scene& rhs) = default;
		Scene& operator=(Scene&& rhs) = default;

		virtual void OnInit();
		virtual void OnRun();
		virtual void OnRelease();

		virtual void OnWindowResizeEvent(WindowResizeEvent e);
		virtual void OnWindowMoveEvent(WindowMoveEvent e);
		virtual void OnWindowCloseEvent(WindowCloseEvent e);
		virtual void OnMouseMoveEvent(MouseMoveEvent e);
		virtual void OnMouseScrollEvent(MouseScrollEvent e);
		virtual void OnMouseButtonEvent(MouseButtonEvent e);
		virtual void OnKeyEvent(KeyEvent e);
		virtual void OnCharEvent(CharEvent e);
	protected:
		std::unique_ptr<Window> m_Window;
		//std::unique_ptr<Renderer> m_Graphics;
	private:
		void SetCallbacks();
	};

}

#endif
