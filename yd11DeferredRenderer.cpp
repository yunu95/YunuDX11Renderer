#include "yd11DeferredRenderer.h"
#include "yd11Texture.h"
#include "ydD11Context.h"

namespace dx11
{
    bool DeferredRenderer::Init()
    {
        // Define a sampler state description
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Example filter mode
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // Example addressing mode for U coordinate
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // Example addressing mode for V coordinate
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // Example addressing mode for W coordinate
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samplerDesc.BorderColor[0] = 0;
        samplerDesc.BorderColor[1] = 0;
        samplerDesc.BorderColor[2] = 0;
        samplerDesc.BorderColor[3] = 0;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        // Create the sampler state object on the CPU
        HRESULT hr = Context::Instance().device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());
        assert(SUCCEEDED(hr));
        return true;
    }
    bool DeferredRenderer::OnResize()
    {
        gBufferPosition = CreateGBuffer(DXGI_FORMAT_R32G32B32A32_FLOAT);
        gBufferNormal = CreateGBuffer(DXGI_FORMAT_R32G32B32A32_FLOAT);
        gBufferAlbedo = CreateGBuffer(DXGI_FORMAT_R8G8B8A8_UNORM);
        gBufferMetallic = CreateGBuffer(DXGI_FORMAT_R8_UNORM);
        gBufferRoughness = CreateGBuffer(DXGI_FORMAT_R8_UNORM);
        gBufferAmbientOcclusion = CreateGBuffer(DXGI_FORMAT_R8G8B8A8_UNORM);
        Context::Instance().device->CreateRenderTargetView(gBufferPosition.Get(), nullptr, gBufferViewPosition.GetAddressOf());
        Context::Instance().device->CreateRenderTargetView(gBufferNormal.Get(), nullptr, gBufferViewNormal.GetAddressOf());
        Context::Instance().device->CreateRenderTargetView(gBufferAlbedo.Get(), nullptr, gBufferViewAlbedo.GetAddressOf());
        Context::Instance().device->CreateRenderTargetView(gBufferMetallic.Get(), nullptr, gBufferViewMetallic.GetAddressOf());
        Context::Instance().device->CreateRenderTargetView(gBufferRoughness.Get(), nullptr, gBufferViewRoughness.GetAddressOf());
        Context::Instance().device->CreateRenderTargetView(gBufferAmbientOcclusion.Get(), nullptr, gBufferViewAmbientOcclusion.GetAddressOf());
        Context::Instance().device->CreateShaderResourceView(gBufferPosition.Get(), nullptr, gBufferSRVPosition.GetAddressOf());
        Context::Instance().device->CreateShaderResourceView(gBufferNormal.Get(), nullptr, gBufferSRVNormal.GetAddressOf());
        Context::Instance().device->CreateShaderResourceView(gBufferAlbedo.Get(), nullptr, gBufferSRVAlbedo.GetAddressOf());
        Context::Instance().device->CreateShaderResourceView(gBufferMetallic.Get(), nullptr, gBufferSRVMetallic.GetAddressOf());
        Context::Instance().device->CreateShaderResourceView(gBufferRoughness.Get(), nullptr, gBufferSRVRoughness.GetAddressOf());
        Context::Instance().device->CreateShaderResourceView(gBufferAmbientOcclusion.Get(), nullptr, gBufferSRVAmbientOcclusion.GetAddressOf());

        gBufferRTVs[0] = gBufferViewPosition.Get();
        gBufferRTVs[1] = gBufferViewNormal.Get();
        gBufferRTVs[2] = gBufferViewAlbedo.Get();
        gBufferRTVs[3] = gBufferViewMetallic.Get();
        gBufferRTVs[4] = gBufferViewRoughness.Get();
        gBufferRTVs[5] = gBufferViewAmbientOcclusion.Get();

        gBufferSRVs[0] = gBufferSRVPosition.Get();
        gBufferSRVs[1] = gBufferSRVNormal.Get();
        gBufferSRVs[2] = gBufferSRVAlbedo.Get();
        gBufferSRVs[3] = gBufferSRVMetallic.Get();
        gBufferSRVs[4] = gBufferSRVRoughness.Get();
        gBufferSRVs[5] = gBufferSRVAmbientOcclusion.Get();

        for (int i = 0; i < 6; i++)
        {
            deferredDebugVertices[i * 6 + 0] = { .x = -1 + gBufferDebugViewportScale * i,.y = 1 - gBufferDebugViewportScale ,.u = 0,.v = 1,.textureIndex = i };
            deferredDebugVertices[i * 6 + 1] = { .x = -1 + gBufferDebugViewportScale * i,.y = 1,.u = 0,.v = 0,.textureIndex = i };
            deferredDebugVertices[i * 6 + 2] = { .x = -1 + gBufferDebugViewportScale * (i + 1),.y = 1,.u = 1,.v = 0,.textureIndex = i };
            deferredDebugVertices[i * 6 + 3] = { .x = -1 + gBufferDebugViewportScale * i,.y = 1 - gBufferDebugViewportScale,.u = 0,.v = 1,.textureIndex = i };
            deferredDebugVertices[i * 6 + 4] = { .x = -1 + gBufferDebugViewportScale * (i + 1),.y = 1,.u = 1,.v = 0,.textureIndex = i };
            deferredDebugVertices[i * 6 + 5] = { .x = -1 + gBufferDebugViewportScale * (i + 1),.y = 1 - gBufferDebugViewportScale,.u = 1,.v = 1,.textureIndex = i };
        }

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(deferredDebugVertices);
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = deferredDebugVertices;

        Context::Instance().device->CreateBuffer(&bufferDesc, &initData, deferredDebugVerticesBuffer.GetAddressOf());

        return true;
    }
    void DeferredRenderer::RenderStaticMeshes()
    {
        static auto arm = ResourceManager::Instance().LoadTexture(L"T_Hat_ARM.png");
        static auto albedo = ResourceManager::Instance().LoadTexture(L"T_Hat_BaseColor.png");
        static auto normal = ResourceManager::Instance().LoadTexture(L"T_Hat_Normal.png");
        static auto vertexShader = ResourceManager::Instance().LoadVertexShaderFromFile(L"ydStaticMeshVertexShader.cso");
        static auto pixelShader = ResourceManager::Instance().LoadPixelShaderFromFile(L"ydStaticMeshPixelShader.cso");

        static ID3D11ShaderResourceView* materialSRVs[3] = { albedo->srv.Get(),normal->srv.Get(),arm->srv.Get() };
        for (auto& each : gBufferRTVs)
            Context::Instance().deviceContext->ClearRenderTargetView(each, Color{ 0,0,0,0 });
        Context::Instance().deviceContext->PSSetShaderResources(0, 3, materialSRVs);
        Context::Instance().deviceContext->PSSetShaderResources(0, 3, materialSRVs);

        Context::Instance().deviceContext->OMSetRenderTargets(gBufferCount, gBufferRTVs, Context::Instance().depthStencilView.Get());
        Context::Instance().deviceContext->IASetInputLayout(ResourceManager::Instance().GetInputLayout(vertexShader));
        Context::Instance().deviceContext->VSSetShader(vertexShader, nullptr, 0);
        Context::Instance().deviceContext->PSSetShader(pixelShader, nullptr, 0);
        Context::Instance().deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        auto fbxScene = ResourceManager::Instance().LoadFBXFile("SM_Hat02.fbx");
        TestDrawNodeRecursively(fbxScene->mRootNode, fbxScene);
    }
    void DeferredRenderer::TestDrawNodeRecursively(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            static constexpr unsigned int offset = 0;
            auto mesh = ResourceManager::Instance().GetMesh(scene->mMeshes[node->mMeshes[i]]);
            Context::Instance().deviceContext->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &StaticMesh::vertexStride, &offset);
            Context::Instance().deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
            Context::Instance().deviceContext->DrawIndexed(scene->mMeshes[node->mMeshes[i]]->mNumFaces * 3, 0, 0);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            TestDrawNodeRecursively(node->mChildren[i], scene);
        }
    }
    bool DeferredRenderer::DrawDeferredImagesOnTop()
    {
        auto vs = ResourceManager::Instance().LoadVertexShaderFromFile(L"ydGBufferDebugVertexShader.cso");
        auto ps = ResourceManager::Instance().LoadPixelShaderFromFile(L"ydGBufferDebugPixelShader.cso");
        auto inputLayout = ResourceManager::Instance().GetInputLayout(vs);
        Context::Instance().deviceContext->IASetInputLayout(inputLayout);

        static UINT stride = sizeof(DeferredDebugVertex);
        static UINT offset = 0;
        Context::Instance().deviceContext->ClearDepthStencilView(Context::Instance().depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        Context::Instance().deviceContext->OMSetRenderTargets(1, Context::Instance().renderTargetView.GetAddressOf(), Context::Instance().depthStencilView.Get());

        Context::Instance().deviceContext->PSSetShaderResources(0, 6, gBufferSRVs);

        Context::Instance().deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
        Context::Instance().deviceContext->IASetVertexBuffers(0, 1, deferredDebugVerticesBuffer.GetAddressOf(), &stride, &offset);
        Context::Instance().deviceContext->VSSetShader(vs, nullptr, 0);
        Context::Instance().deviceContext->PSSetShader(ps, nullptr, 0);
        Context::Instance().deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        Context::Instance().deviceContext->Draw(6 * 6, 0);

        static ID3D11ShaderResourceView* nullSRVs[gBufferCount] = { nullptr };
        Context::Instance().deviceContext->PSSetShaderResources(0, 6, nullSRVs);
        return true;
    }
    ComPtr<ID3D11Texture2D> DeferredRenderer::CreateGBuffer(DXGI_FORMAT format)
    {
        D3D11_TEXTURE2D_DESC desc{
            .Width = Context::Instance().Window_Width,
            .Height = Context::Instance().Window_Height,
            .MipLevels = 1,
            .ArraySize = 1,
            .Format = format,
            .SampleDesc = {1, 0},
            .Usage = D3D11_USAGE_DEFAULT,
            .BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
            .CPUAccessFlags = 0,
            .MiscFlags = 0
        };
        ComPtr<ID3D11Texture2D> texture2D;
        HRESULT hr = Context::Instance().device->CreateTexture2D(&desc, nullptr, texture2D.GetAddressOf());
        assert(SUCCEEDED(hr));
        return texture2D;
    }
}
