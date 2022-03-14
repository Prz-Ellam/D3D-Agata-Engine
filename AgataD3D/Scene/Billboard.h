#ifndef AGATA_D3D_BILLBOARD_H
#define AGATA_D3D_BILLBOARD_H
#pragma once

#include "Drawable.h"
#include "Mesh.h"
#include "ConstantBuffer.h"

namespace Agata {

	HLSL struct BillboardBuffer {

	};

	class Billboard : public Drawable {
	public:
		Billboard(const std::string path, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);
		Billboard(const Billboard& other) = default;
		Billboard(Billboard&& other) noexcept = default;
		virtual ~Billboard() = default;

		Billboard& operator=(const Billboard& other) = default;
		Billboard& operator=(Billboard&& other) noexcept = default;

		void OnRender() override;
	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<ConstantBuffer> m_CBO;
		BillboardBuffer m_Buffer;
	};

}

#endif
