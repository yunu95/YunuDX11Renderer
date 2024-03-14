#include "yd11StaticMesh.h"
#include "ydD11Context.h"

namespace dx11
{
    void StaticMesh::SetVertexVec(const std::vector<StaticMeshVertex>& vertexVec)
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
        vertexCount = static_cast<UINT>(vertexVec.size());
    };
    void StaticMesh::SetIndexVec(const std::vector<unsigned int>& indexVec)
    {
        this->indexVec = std::move(indexVec);
        D3D11_BUFFER_DESC bufferDesc
        {
            .ByteWidth = static_cast<UINT>(indexVec.size() * indexStride),
            .Usage = D3D11_USAGE_DEFAULT,
            .BindFlags = D3D11_BIND_INDEX_BUFFER,
            .CPUAccessFlags = 0,
        };
        D3D11_SUBRESOURCE_DATA initData{ .pSysMem = indexVec.data() };
        Context::Instance().device->CreateBuffer(&bufferDesc, &initData, indexBuffer.GetAddressOf());
        indexCount = static_cast<UINT>(indexVec.size());
    };
    void StaticMesh::SetAiMesh(aiMesh* aimesh)
    {
        this->aimesh = aimesh;
    }
    aiMesh* StaticMesh::GetaiMesh()
    {
        return aimesh;
    }
}