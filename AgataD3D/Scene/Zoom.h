#ifndef AGATA_D11_ZOOM_H
#define AGATA_D11_ZOOM_H
#pragma once

#include "Mesh.h"
#include "Texture2D.h"
#include <memory>

namespace Agata {

	class Zoom {
	public:
		Zoom(const std::string& shape);
		Zoom(const Zoom& other) = delete;
		Zoom(Zoom&& other) noexcept = delete;
		virtual ~Zoom() = default;

		Zoom& operator=(const Zoom& other) = delete;
		Zoom& operator=(Zoom&& other) noexcept = delete;

		void OnRender();
	private:
		std::shared_ptr<Mesh> m_Mesh;
		Texture2D m_Shape;
	};

}

#endif
