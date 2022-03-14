#ifndef AGATA_D11_MATH_UTILS_H
#define AGATA_D11_MATH_UTILS_H
#pragma once

#include <DirectXMath.h>

namespace DX = DirectX;

namespace Agata {

	class MathUtils {
	public:
		MathUtils() = delete;
		MathUtils(const MathUtils& other) = delete;
		MathUtils(MathUtils&& other) noexcept = delete;

		static float BaryCentricCoordinatesY(const DX::XMFLOAT3& p1, const DX::XMFLOAT3& p2, 
			const DX::XMFLOAT3& p3, const DX::XMFLOAT2& pos);
	};

}

#endif
