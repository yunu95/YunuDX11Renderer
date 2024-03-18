#include "ydGrid.h"
#include "yd11Context.h"

namespace dx11
{
    Grid::Grid()
    {
        static const D3D11_RASTERIZER_DESC rasterizerDesc
        {
            .FillMode = D3D11_FILL_SOLID,
            .CullMode = D3D11_CULL_NONE,
        };
        Context::Instance().device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());
        vs = ResourceManager::Instance().LoadVertexShaderFromFile(L"yd11ColoredVertexShader.cso");
        ps = ResourceManager::Instance().LoadPixelShaderFromFile(L"ydColoredPixelShader.cso");
        inputLayout = ResourceManager::Instance().GetInputLayout(vs);
        // 그리드 버텍스
        for (int x = -gridHalfCoverage; x <= gridHalfCoverage; x++)
        {
            if (x == 0)
            {
                vertices.push_back(primitives::ColoredVertex{ .x = x * gridLength,.y = 0,.z = -gridHalfCoverage * gridLength,.color = gridColor, });
                vertices.push_back(primitives::ColoredVertex{ .x = x * gridLength,.y = 0,.z = 0 ,.color = gridColor, });
            }
            else
            {
                vertices.push_back(primitives::ColoredVertex{ .x = x * gridLength,.y = 0,.z = -gridHalfCoverage * gridLength,.color = gridColor, });
                vertices.push_back(primitives::ColoredVertex{ .x = x * gridLength,.y = 0,.z = +gridHalfCoverage * gridLength,.color = gridColor, });
            }
        }
        for (int z = -gridHalfCoverage; z <= gridHalfCoverage; z++)
        {
            if (z == 0)
            {
                vertices.push_back(primitives::ColoredVertex{ .x = -gridHalfCoverage * gridLength,.y = 0,.z = z * gridLength,.color = gridColor, });
                vertices.push_back(primitives::ColoredVertex{ .x = 0,.y = 0,.z = z * gridLength,.color = gridColor, });
            }
            else
            {
                vertices.push_back(primitives::ColoredVertex{ .x = -gridHalfCoverage * gridLength,.y = 0,.z = z * gridLength,.color = gridColor, });
                vertices.push_back(primitives::ColoredVertex{ .x = +gridHalfCoverage * gridLength,.y = 0,.z = z * gridLength,.color = gridColor, });
            }
        }
        // 기즈모 버텍스
        vertices.push_back(primitives::ColoredVertex{ .x = 0,.y = 0,.z = 0,.color = {1,0,0,1} });
        vertices.push_back(primitives::ColoredVertex{ .x = 12345,.y = 0,.z = 0,.color = {1,0,0,1} });
        vertices.push_back(primitives::ColoredVertex{ .x = 0,.y = 0,.z = 0,.color = {0,1,0,1} });
        vertices.push_back(primitives::ColoredVertex{ .x = 0,.y = 12345,.z = 0,.color = {0,1,0,1} });
        vertices.push_back(primitives::ColoredVertex{ .x = 0,.y = 0,.z = 0,.color = {0,0,1,1} });
        vertices.push_back(primitives::ColoredVertex{ .x = 0,.y = 0,.z = 12345,.color = {0,0,1,1} });
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(primitives::ColoredVertex));
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vertices.data();

        Context::Instance().device->CreateBuffer(&bufferDesc, &initData, vertexBuffer.GetAddressOf());

        // Set vertex buffer
    }
    void Grid::DrawGridAndGuizmo()
    {
        Context::Instance().deviceContext->OMSetRenderTargets(1, Context::Instance().renderTargetView.GetAddressOf(), Context::Instance().depthStencilView.Get());
        Context::Instance().deviceContext->RSSetState(rasterizerState.Get());
        Context::Instance().deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        Context::Instance().deviceContext->IASetInputLayout(inputLayout);
        Context::Instance().deviceContext->VSSetShader(vs, nullptr, 0);
        Context::Instance().deviceContext->PSSetShader(ps, nullptr, 0);
        UINT stride = sizeof(primitives::ColoredVertex);
        UINT offset = 0;
        Context::Instance().deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
        Context::Instance().deviceContext->Draw(static_cast<UINT>(vertices.size()), 0);
    }
}
