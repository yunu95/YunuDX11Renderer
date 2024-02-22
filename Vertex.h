#pragma once
#include "DX11Dependency.h"

namespace primitives
{
	struct Vertex {
		float x, y, z;
		DirectX::XMFLOAT4 color;
	};
}