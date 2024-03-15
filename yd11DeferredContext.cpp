#include "yd11DeferredContext.h"
#include "yd11Texture.h"
#include "ydD11Context.h"

namespace dx11
{
    bool DeferredContext::Init()
    {
        return true;
    }
    bool DeferredContext::OnResize()
    {
        gBufferPosition = CreateGBuffer(DXGI_FORMAT_R32G32B32A32_FLOAT);
        gBufferNormal = CreateGBuffer(DXGI_FORMAT_R32G32B32A32_FLOAT);
        gBufferAlbedo = CreateGBuffer(DXGI_FORMAT_R8G8B8A8_UNORM);
        gBufferARM = CreateGBuffer(DXGI_FORMAT_R8G8B8A8_UNORM);
        Context::Instance().device->CreateRenderTargetView(gBufferPosition.Get(), nullptr, gBufferViewPosition.GetAddressOf());
        Context::Instance().device->CreateRenderTargetView(gBufferNormal.Get(), nullptr, gBufferViewNormal.GetAddressOf());
        Context::Instance().device->CreateRenderTargetView(gBufferAlbedo.Get(), nullptr, gBufferViewAlbedo.GetAddressOf());
        Context::Instance().device->CreateRenderTargetView(gBufferARM.Get(), nullptr, gBufferViewARM.GetAddressOf());
        Context::Instance().device->CreateShaderResourceView(gBufferPosition.Get(), nullptr, gBufferSRVPosition.GetAddressOf());
        Context::Instance().device->CreateShaderResourceView(gBufferNormal.Get(), nullptr, gBufferSRVNormal.GetAddressOf());
        Context::Instance().device->CreateShaderResourceView(gBufferAlbedo.Get(), nullptr, gBufferSRVAlbedo.GetAddressOf());
        Context::Instance().device->CreateShaderResourceView(gBufferARM.Get(), nullptr, gBufferSRVARM.GetAddressOf());

        gBufferRTVs[0] = gBufferViewPosition.Get();
        gBufferRTVs[1] = gBufferViewNormal.Get();
        gBufferRTVs[2] = gBufferViewAlbedo.Get();
        gBufferRTVs[3] = gBufferViewARM.Get();

        gBufferSRVs[0] = gBufferSRVPosition.Get();
        gBufferSRVs[1] = gBufferSRVNormal.Get();
        gBufferSRVs[2] = gBufferSRVAlbedo.Get();
        gBufferSRVs[3] = gBufferSRVARM.Get();

        // 뎁스 스텐실 버퍼와 뷰 생성
        ComPtr<ID3D11Texture2D> depthStencilBuffer;		// 뎁스 스탠실 버퍼
        D3D11_TEXTURE2D_DESC depthStencilDesc
        {
            .Width = Context::Instance().Window_Width,
            .Height = Context::Instance().Window_Height,
            .MipLevels = 1,
            .ArraySize = 1,
            .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
            .SampleDesc = Context::Instance().multiSamplingDesc,
            .Usage = D3D11_USAGE_DEFAULT,
            .BindFlags = D3D11_BIND_DEPTH_STENCIL,
            .CPUAccessFlags = 0,
            .MiscFlags = 0
        };
        Context::Instance().device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf());
        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{
            .Format = depthStencilDesc.Format,
            .ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D,
            .Texture2D = {0}
        };
        D3D11_DEPTH_STENCIL_DESC desc{
            .DepthEnable = TRUE,
            .DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
            .DepthFunc = D3D11_COMPARISON_LESS,
            .StencilEnable = FALSE
        };

        //HRESULT hr = Context::Instance().device->CreateDepthStencilState(&desc, depthStencilState.GetAddressOf());
        //assert(SUCCEEDED(hr));
        //Context::Instance().device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, depthStencilView.GetAddressOf());

        // 화면 상단에 g버퍼 이미지들을 그리기 위한 버텍스 버퍼 생성
        {
            primitives::DeferredDebugVertex vertices[6 * gBufferCount]{};
            for (int i = 0; i < gBufferCount; i++)
            {
                vertices[i * 6 + 0] = { .x = -1 + gBufferDebugViewportScale * i,.y = 1 - gBufferDebugViewportScale ,.u = 0,.v = 1,.textureIndex = i };
                vertices[i * 6 + 1] = { .x = -1 + gBufferDebugViewportScale * i,.y = 1,.u = 0,.v = 0,.textureIndex = i };
                vertices[i * 6 + 2] = { .x = -1 + gBufferDebugViewportScale * (i + 1),.y = 1,.u = 1,.v = 0,.textureIndex = i };
                vertices[i * 6 + 3] = { .x = -1 + gBufferDebugViewportScale * i,.y = 1 - gBufferDebugViewportScale,.u = 0,.v = 1,.textureIndex = i };
                vertices[i * 6 + 4] = { .x = -1 + gBufferDebugViewportScale * (i + 1),.y = 1,.u = 1,.v = 0,.textureIndex = i };
                vertices[i * 6 + 5] = { .x = -1 + gBufferDebugViewportScale * (i + 1),.y = 1 - gBufferDebugViewportScale,.u = 1,.v = 1,.textureIndex = i };
            }

            D3D11_BUFFER_DESC bufferDesc = {};
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.ByteWidth = sizeof(vertices);
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.CPUAccessFlags = 0;
            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = vertices;

            Context::Instance().device->CreateBuffer(&bufferDesc, &initData, debugGbufferVerticesBuffer.GetAddressOf());
        }

        return true;
    }
    void DeferredContext::RenderStaticMeshes()
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
    void DeferredContext::TestDrawNodeRecursively(aiNode* node, const aiScene* scene)
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
    void DeferredContext::RenderToBackBuffer()
    {
        static auto vs = ResourceManager::Instance().LoadVertexShaderFromFile(L"ydLightVS.cso");
        static auto ps = ResourceManager::Instance().LoadPixelShaderFromFile(L"ydLightPS.cso");
        auto inputLayout = ResourceManager::Instance().GetInputLayout(vs);
        Context::Instance().deviceContext->IASetInputLayout(inputLayout);

        static UINT stride = sizeof(primitives::ScreenPosUVVertex);
        static UINT offset = 0;
        Context::Instance().deviceContext->OMSetRenderTargets(1, Context::Instance().renderTargetView.GetAddressOf(), nullptr);
        Context::Instance().deviceContext->PSSetShaderResources(0, gBufferCount, gBufferSRVs);
        Context::Instance().deviceContext->PSSetSamplers(0, 1, Context::Instance().defaultSamplerState.GetAddressOf());
        Context::Instance().deviceContext->IASetVertexBuffers(0, 1, Context::Instance().screenPosUVVertexBuffer.GetAddressOf(), &stride, &offset);
        Context::Instance().deviceContext->IASetIndexBuffer(Context::Instance().screenPosUVIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        Context::Instance().deviceContext->VSSetShader(vs, nullptr, 0);
        Context::Instance().deviceContext->PSSetShader(ps, nullptr, 0);
        Context::Instance().deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        Context::Instance().deviceContext->DrawIndexed(6, 0, 0);

        static ID3D11ShaderResourceView* nullSRVs[gBufferCount] = { nullptr };
        Context::Instance().deviceContext->PSSetShaderResources(0, gBufferCount, nullSRVs);
    }
    bool DeferredContext::DrawDeferredImagesOnTop()
    {
        auto vs = ResourceManager::Instance().LoadVertexShaderFromFile(L"ydGBufferDebugVertexShader.cso");
        auto ps = ResourceManager::Instance().LoadPixelShaderFromFile(L"ydGBufferDebugPixelShader.cso");
        auto inputLayout = ResourceManager::Instance().GetInputLayout(vs);
        Context::Instance().deviceContext->IASetInputLayout(inputLayout);

        static UINT stride = sizeof(DeferredDebugVertex);
        static UINT offset = 0;
        //Context::Instance().deviceContext->ClearDepthStencilView(Context::Instance().depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        //Context::Instance().deviceContext->OMSetRenderTargets(1, Context::Instance().renderTargetView.GetAddressOf(), Context::Instance().depthStencilView.Get());
        Context::Instance().deviceContext->OMSetRenderTargets(1, Context::Instance().renderTargetView.GetAddressOf(), nullptr);

        Context::Instance().deviceContext->PSSetShaderResources(0, gBufferCount, gBufferSRVs);

        Context::Instance().deviceContext->PSSetSamplers(0, 1, Context::Instance().defaultSamplerState.GetAddressOf());
        Context::Instance().deviceContext->IASetVertexBuffers(0, 1, debugGbufferVerticesBuffer.GetAddressOf(), &stride, &offset);
        Context::Instance().deviceContext->VSSetShader(vs, nullptr, 0);
        Context::Instance().deviceContext->PSSetShader(ps, nullptr, 0);
        Context::Instance().deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        Context::Instance().deviceContext->Draw(6 * gBufferCount, 0);

        static ID3D11ShaderResourceView* nullSRVs[gBufferCount] = { nullptr };
        Context::Instance().deviceContext->PSSetShaderResources(0, gBufferCount, nullSRVs);
        return true;
    }
    ComPtr<ID3D11Texture2D> DeferredContext::CreateGBuffer(DXGI_FORMAT format)
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
