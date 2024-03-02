#pragma once

#include "ydDX11Dependency.h"
#include "ydColoredVertex.h"
#include <vector>

namespace dx11
{
    using namespace Microsoft::WRL;
    class Grid
    {
    public:
        // x,y,z축과 그리드를 그림
        // 그리드의 한 변의 크기
        static constexpr float gridLength{ 1 };
        // xz축에 배치된 그리드들의 갯수, 원점을 중심으로 xz평면에 gridHalfCoverage * 10만큼 존재
        static constexpr int gridHalfCoverage{ 10 };
        // 그리드 색깔
        static constexpr DirectX::XMFLOAT4 gridColor{ 1,1,1,1 };
        Grid();
        
        void DrawGridAndGuizmo();
        
    private:
        ComPtr<ID3D11RasterizerState> rasterizerState;
        ID3D11VertexShader* vs;
        ID3D11PixelShader* ps;
        ID3D11InputLayout* inputLayout;
        std::vector<primitives::ColoredVertex> vertices;
        ComPtr<ID3D11Buffer> vertexBuffer;
    };
}
