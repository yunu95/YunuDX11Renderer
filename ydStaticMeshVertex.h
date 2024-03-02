#pragma once
#include "ydDX11Dependency.h"

namespace dx11
{
    struct StaticMeshVertex
    {
        DirectX::SimpleMath::Vector3 pos; byte pad0[4];
        DirectX::SimpleMath::Vector2 uv; byte pad1[8];
        DirectX::SimpleMath::Vector3 normal; byte pad2[4];
        DirectX::SimpleMath::Vector3 tangent; byte pad3[4];
        float metallic; byte pad4[12];
        float roughness; byte pad5[12];
    };
}
