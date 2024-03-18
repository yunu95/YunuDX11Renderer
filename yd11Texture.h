#pragma once
#include "yd11DXDependency.h"

namespace dx11
{
    class Texture
    {
    public:
        Texture() = default;
        ComPtr<ID3D11Resource> texture;
        ComPtr<ID3D11ShaderResourceView> srv;
    };
}
