#include "ydDX11Dependency.h"
#include "ydD11ResourceManager.h"
#include "ydD11Context.h"
#include "yd11CubeMap.h"

namespace dx11
{
    void CubeMap::RenderToBackBuffer()
    {
        static auto vs = ResourceManager::Instance().LoadVertexShaderFromFile(L"ydCubeMapVS.cso");
        static auto ps = ResourceManager::Instance().LoadPixelShaderFromFile(L"ydCubeMapPS.cso");
        auto inputLayout = ResourceManager::Instance().GetInputLayout(vs);
        Context::Instance().deviceContext->IASetInputLayout(inputLayout);

        static UINT stride = sizeof(primitives::ScreenPosUVVertex);
        static UINT offset = 0;
        Context::Instance().deviceContext->OMSetRenderTargets(1, Context::Instance().renderTargetView.GetAddressOf(), Context::Instance().depthStencilView.Get());
        //Context::Instance().deviceContext->PSSetShaderResources(0, gBufferCount, gBufferSRVs);
        Context::Instance().deviceContext->PSSetSamplers(0, 1, Context::Instance().defaultSamplerState.GetAddressOf());
        Context::Instance().deviceContext->IASetVertexBuffers(0, 1, Context::Instance().screenPosUVVertexBuffer.GetAddressOf(), &stride, &offset);
        Context::Instance().deviceContext->IASetIndexBuffer(Context::Instance().screenPosUVIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        Context::Instance().deviceContext->VSSetShader(vs, nullptr, 0);
        Context::Instance().deviceContext->PSSetShader(ps, nullptr, 0);
        Context::Instance().deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        Context::Instance().deviceContext->DrawIndexed(6, 0, 0);

        //static ID3D11ShaderResourceView* nullSRVs[gBufferCount] = { nullptr };
        //Context::Instance().deviceContext->PSSetShaderResources(0, gBufferCount, nullSRVs);
    }
}