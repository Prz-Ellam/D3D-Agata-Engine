#include "pch.h"
#include "Scene.h"

namespace Agata {

	Scene::Scene(const WindowParams& windowParams) {

		m_Window = std::make_unique<Window>(windowParams);
		SetCallbacks();

		// Iniciamos Direct3D 11 con el metodo Init del Renderer
		Renderer::Init(m_Window);

	}

	Scene::~Scene() {

		Renderer::Release();

	}

	void Scene::OnInit() {}

	void Scene::OnRun() {}

	void Scene::OnRelease() {}

	void Scene::SetCallbacks() {

		m_Window->SetWindowResizeEventHandler(EVENT_FN(Scene::OnWindowResizeEvent));
		m_Window->SetWindowMoveEventHandler(EVENT_FN(Scene::OnWindowMoveEvent));
		m_Window->SetWindowCloseEventHandler(EVENT_FN(Scene::OnWindowCloseEvent));
		m_Window->SetMouseMoveEventHandler(EVENT_FN(Scene::OnMouseMoveEvent));
		m_Window->SetMouseScrollEventHandler(EVENT_FN(Scene::OnMouseScrollEvent));
		m_Window->SetMouseButtonEventHandler(EVENT_FN(Scene::OnMouseButtonEvent));
		m_Window->SetKeyEventHandler(EVENT_FN(Scene::OnKeyEvent));
		m_Window->SetCharEventHandler(EVENT_FN(Scene::OnCharEvent));

	}

	void Scene::OnWindowResizeEvent(WindowResizeEvent e) {}

	void Scene::OnWindowMoveEvent(WindowMoveEvent e) {}

	void Scene::OnWindowCloseEvent(WindowCloseEvent e) {}

	void Scene::OnMouseMoveEvent(MouseMoveEvent e) {}

	void Scene::OnMouseScrollEvent(MouseScrollEvent e) {}

	void Scene::OnMouseButtonEvent(MouseButtonEvent e) {}

	void Scene::OnKeyEvent(KeyEvent e) {}

	void Scene::OnCharEvent(CharEvent e) {}

}