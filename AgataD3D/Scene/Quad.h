#ifndef AGATA_D11_QUAD_H
#define AGATA_D11_QUAD_H
#pragma once

#include "Mesh.h"
#include "Loader.h"
#include "Renderer.h"

namespace Agata {

	class Quad {
	public:
		Quad() {
			m_Mesh = Loader::Get().LoadQuad();
		}
		void Bind() {
			Renderer::DrawIndexes(m_Mesh.get());
		}
	private:
		std::shared_ptr<Mesh> m_Mesh;
	};

}

#endif
