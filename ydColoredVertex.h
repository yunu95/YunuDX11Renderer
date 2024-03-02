#pragma once
#include "ydDX11Dependency.h"

namespace primitives
{
    struct ColoredVertex {
        float x, y, z;
        byte padding[4];
        DirectX::XMFLOAT4 color;
    };
}