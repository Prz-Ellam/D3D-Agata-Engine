#include "pch.h"
#include "Zoom.h"
#include "Loader.h"

namespace Agata {

	Zoom::Zoom(const std::string& shape) : m_Shape(shape) {

		m_Mesh = Loader::Get().LoadQuad();

	}

	void Zoom::OnRender() {

		m_Shape.Bind(0);
		Renderer::DrawIndexes(m_Mesh.get());

	}

}