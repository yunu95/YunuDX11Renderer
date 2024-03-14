#pragma once
#include "ydStaticMeshVertex.h"
#include "ydDX11Dependency.h"

namespace dx11
{
    // StaticMesh는 인풋으로 쓰이는 버텍스, 인덱스 버퍼 정보만을 저장한다.
    // 메시에 관한 다른 기타등등 정보들은 aiMesh에서 가져오면 된다.
    class StaticMesh
    {
    public:
        static constexpr UINT vertexStride{ sizeof(StaticMeshVertex) };
        static constexpr UINT indexStride{ sizeof(unsigned int) };
        //const std::vector<StaticMeshVertex>& GetVertexVec() { return vertexVec; };
        //const std::vector<unsigned int>& GetIndexVec() { return indexVec; };
        const ComPtr<ID3D11Buffer>& GetVertexBuffer() { return vertexBuffer; };
        const ComPtr<ID3D11Buffer>& GetIndexBuffer() { return indexBuffer; };
        /*ID3D11Buffer* const* GetAddressOfVertexBuffer() { return vertexBuffer.GetAddressOf(); };
        ID3D11Buffer* const* GetAddressOfIndexBuffer() { return indexBuffer.GetAddressOf(); };
        unsigned int GetVertexCount() { return vertexCount; };
        unsigned int GetIndexCount() { return indexCount; };*/
        void SetVertexVec(const std::vector<StaticMeshVertex>& vertexVec);
        void SetIndexVec(const std::vector<unsigned int>& indexVec);
        void SetAiMesh(aiMesh* aimesh);
        aiMesh* GetaiMesh();
    private:
        std::vector<unsigned int> indexVec;
        Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
        aiMesh* aimesh{ nullptr };
        unsigned int vertexCount{ 0 };
        unsigned int indexCount{ 0 };

        unsigned int materialCount{ 0 };
    };

}
