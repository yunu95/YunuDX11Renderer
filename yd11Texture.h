#pragma once
#include "ydDX11Dependency.h"

namespace dx11
{
    class Texture
    {
    public:
        Texture() = default;
        ComPtr<ID3D11ShaderResourceView> srv;
        //ComPtr<ID3D11Texture2D> texture;
    };
}
