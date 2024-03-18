#include "yd11CubeMap.h"
#include "yd11DXDependency.h"
#include "yd11ResourceManager.h"
#include "yd11CubeMapMeshVertex.h"
#include "yd11Context.h"

namespace dx11
{
    CubeMap::CubeMap()
    {
    }
    void CubeMap::RenderToBackBuffer()
    {
        static auto vs = ResourceManager::Instance().LoadVertexShaderFromFile(L"yd11CubeMapVS.cso");
        static auto ps = ResourceManager::Instance().LoadPixelShaderFromFile(L"yd11CubeMapPS.cso");
        auto inputLayout = ResourceManager::Instance().GetInputLayout(vs);
        Context::Instance().deviceContext->IASetInputLayout(inputLayout);

        static UINT stride = sizeof(primitives::CubeMapMeshVertex);
        static UINT offset = 0;
        Context::Instance().deviceContext->OMSetRenderTargets(1, Context::Instance().renderTargetView.GetAddressOf(), Context::Instance().depthStencilView.Get());
        Context::Instance().deviceContext->PSSetShaderResources(0, 1, texture->srv.GetAddressOf());
        Context::Instance().deviceContext->PSSetSamplers(0, 1, Context::Instance().defaultSamplerState.GetAddressOf());
        Context::Instance().deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
        Context::Instance().deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        Context::Instance().deviceContext->VSSetShader(vs, nullptr, 0);
        Context::Instance().deviceContext->PSSetShader(ps, nullptr, 0);
        Context::Instance().deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        Context::Instance().deviceContext->RSSetState(Context::Instance().nonCullingRasterizerState.Get());
        Context::Instance().deviceContext->OMSetDepthStencilState(Context::Instance().lessEqualDepthStencilState.Get(), 0);
        Context::Instance().deviceContext->DrawIndexed(indexCount, 0, 0);
    }
    void CubeMap::SetCubeMapMesh(aiMesh* meshInfo)
    {
        this->meshInfo = meshInfo;
        vector<CubeMapMeshVertex> vertexVec;
        vector<unsigned int> indexVec;
        for (unsigned int i = 0; i < meshInfo->mNumVertices; i++)
        {
            auto& vertex = meshInfo->mVertices[i];
            vertexVec.push_back({ .x = vertex.x,.y = vertex.y,.z = vertex.z });
        }
        for (unsigned int i = 0; i < meshInfo->mNumFaces; i++)
        {
            auto& face = meshInfo->mFaces[i];
            assert(meshInfo->mFaces[i].mNumIndices == 3 && "페이스를 구성하는 인덱스의 갯수는 3개여야 합니다!");
            indexVec.push_back(face.mIndices[0]);
            indexVec.push_back(face.mIndices[1]);
            indexVec.push_back(face.mIndices[2]);
        }

        {
            D3D11_BUFFER_DESC bufferDesc
            {
                .ByteWidth = static_cast<UINT>(vertexVec.size() * vertexStride),
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_VERTEX_BUFFER,
                .CPUAccessFlags = 0,
            };
            D3D11_SUBRESOURCE_DATA initData{ .pSysMem = vertexVec.data() };
            Context::Instance().device->CreateBuffer(&bufferDesc, &initData, vertexBuffer.GetAddressOf());
            //vertexCount = static_cast<UINT>(vertexVec.size());
        }

        {
            D3D11_BUFFER_DESC bufferDesc
            {
                .ByteWidth = static_cast<UINT>(indexVec.size() * sizeof(unsigned int)),
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_INDEX_BUFFER,
                .CPUAccessFlags = 0,
            };
            D3D11_SUBRESOURCE_DATA initData{ .pSysMem = indexVec.data() };
            Context::Instance().device->CreateBuffer(&bufferDesc, &initData, indexBuffer.GetAddressOf());
            indexCount = static_cast<UINT>(indexVec.size());
        }
    }
}