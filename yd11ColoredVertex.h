#pragma once
#include "yd11DXDependency.h"

namespace dx11::primitives
{
    struct ColoredVertex {
        float x, y, z;
        std::byte padding[4];
        DirectX::XMFLOAT4 color;
    };
}