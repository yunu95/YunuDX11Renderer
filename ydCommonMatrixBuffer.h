#pragma once
#include "ydDX11Dependency.h"

struct CommonMatrixBuffer
{
    DirectX::XMFLOAT4X4 viewMatrix;
    // 투영 변환 행렬
    DirectX::XMFLOAT4X4 projectionMatrix;
};
