#ifndef AGATA_D11_IBINDABLE_H
#define AGATA_D11_IBINDABLE_H
#pragma once

#include <d3d11.h>
#include <stdint.h>

namespace Agata {

	class IBindable {
	public:
		virtual void Bind(uint32_t slot = 0u) const = 0;
	};

}


#endif
