#ifndef AGATA_D11_INPUT_LAYOUT_H
#define AGATA_D11_INPUT_LAYOUT_H
#pragma once

#include <d3d11.h>
#include "BufferLayout.h"
#include <vector>

namespace Agata {

	class InputLayout {
	public:
		InputLayout(std::vector<BufferLayout> layouts);
	private:
		ID3D11InputLayout* m_ID;
	};

}

#endif
