#pragma once
#include "yd11DXDependency.h"

namespace dx11::primitives
{
    struct StaticMeshVertex
    {
        DirectX::SimpleMath::Vector3 pos; std::byte pad0[4];
        DirectX::SimpleMath::Vector3 normal; std::byte pad1[4];
        DirectX::SimpleMath::Vector3 tangent; std::byte pad2[4];
        DirectX::SimpleMath::Vector2 uvAlbedo; std::byte pad3[8];
        DirectX::SimpleMath::Vector2 uvMetallic; std::byte pad4[8];
        DirectX::SimpleMath::Vector2 uvNormal; std::byte pad5[8];
        DirectX::SimpleMath::Vector2 uvRoughness; std::byte pad6[8];
        DirectX::SimpleMath::Vector2 uvAO; std::byte pad7[8];
    };
}
