#pragma once
#include "ydDX11Dependency.h"

namespace dx11
{
    struct StaticMeshVertex
    {
        DirectX::SimpleMath::Vector3 pos; byte pad0[4];
        DirectX::SimpleMath::Vector3 normal; byte pad1[4];
        DirectX::SimpleMath::Vector3 tangent; byte pad2[4];
        DirectX::SimpleMath::Vector2 uvAlbedo; byte pad3[8];
        DirectX::SimpleMath::Vector2 uvMetallic; byte pad4[8];
        DirectX::SimpleMath::Vector2 uvNormal; byte pad5[8];
        DirectX::SimpleMath::Vector2 uvRoughness; byte pad6[8];
        DirectX::SimpleMath::Vector2 uvAO; byte pad7[8];
    };
}
