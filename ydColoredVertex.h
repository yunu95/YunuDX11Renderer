#pragma once
#include "ydDX11Dependency.h"

namespace dx11::primitives
{
    struct ColoredVertex {
        float x, y, z;
        std::byte padding[4];
        DirectX::XMFLOAT4 color;
    };
}