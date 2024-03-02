#pragma once
#include "ydDX11Dependency.h"

namespace primitives
{
    //float2 position : POSITION; // Screen space coordinates
    //float2 uv : TEXCOORD;
    //uint textureIndex : TEXINDEX; // Texture register index
    struct DeferredDebugVertex {
        float x, y;
        byte padding0[8];
        float u, v;
        byte padding1[8];
        int textureIndex;
        byte padding2[12];
    };
}
