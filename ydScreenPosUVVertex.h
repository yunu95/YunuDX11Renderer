#pragma once
#include "ydDX11Dependency.h"

namespace dx11::primitives
{
    //float2 position : POSITION; // Screen space coordinates
    //float2 uv : TEXCOORD;
    //uint textureIndex : TEXINDEX; // Texture register index
    struct ScreenPosUVVertex {
        float x, y;
        std::byte padding0[8];
        float u, v;
        std::byte padding1[8];
    };
}