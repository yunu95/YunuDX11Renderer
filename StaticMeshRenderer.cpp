#include "StaticMeshRenderer.h"
#include "ydD11Context.h"
#include "yd11DeferredContext.h"
#include "yd11Material.h"
#include "yd11StaticMesh.h"

namespace dx11
{
    StaticMeshRenderer::StaticMeshRenderer()
    {
    }
    StaticMeshRenderer::~StaticMeshRenderer()
    {
    }
    void StaticMeshRenderer::Render()
    {
        Context::Instance().deviceContext->PSSetShaderResources(0, 3, material->GetMaterialSRVs().data());
        Context::Instance().deviceContext->OMSetRenderTargets(
            DeferredContext::gBufferCount,
            DeferredContext::Instance().gBufferRTVs,
            Context::Instance().depthStencilView.Get());
        Context::Instance().deviceContext->IASetInputLayout(ResourceManager::Instance().GetInputLayout(material->vertexShader.Get()));
        Context::Instance().deviceContext->VSSetShader(material->vertexShader.Get(), nullptr, 0);
        Context::Instance().deviceContext->PSSetShader(material->pixelShader.Get(), nullptr, 0);
        Context::Instance().deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        static constexpr unsigned int offset = 0;
        Context::Instance().deviceContext->IASetVertexBuffers(0, 1, staticMesh->GetVertexBuffer().GetAddressOf(), &StaticMesh::vertexStride, &offset);
        Context::Instance().deviceContext->IASetIndexBuffer(staticMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
        Context::Instance().deviceContext->DrawIndexed(staticMesh->GetaiMesh()->mNumFaces * 3, 0, 0);
    }
}
