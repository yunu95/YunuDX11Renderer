#pragma once
#include "yd11DXDependency.h"

namespace dx11::primitives
{
    //float2 position : POSITION; // Screen space coordinates
    //float2 uv : TEXCOORD;
    //uint textureIndex : TEXINDEX; // Texture register index
    struct DeferredDebugVertex {
        float x, y;
        std::byte padding0[8];
        float u, v;
        std::byte padding1[8];
        int textureIndex;
        std::byte padding2[12];
    };
}
