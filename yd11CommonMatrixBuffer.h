#pragma once
#include "yd11DXDependency.h"

struct CommonMatrixBuffer
{
    DirectX::XMFLOAT4X4 view;
    // 투영 변환 행렬
    DirectX::XMFLOAT4X4 world;
    // 투영 변환 행렬
    DirectX::XMFLOAT4X4 projection;
    // 뷰 * 투영 변환 행렬
    DirectX::XMFLOAT4X4 viewProjection;
};
